#include "../../interpreter/PyTrackingAlgorithm.h"

namespace BioTracker {
namespace Core {
namespace Interpreter {

PyTrackingAlgorithm::PyTrackingAlgorithm(QString moduleName, Settings &s,
        QWidget *p) :
    TrackingAlgorithm(s, p),
    m_pModule(PyImport_ImportModule(moduleName.toStdString().data())) {

    // validate the module

    m_pTrackFunc = PyObject_GetAttrString(m_pModule, "track");
    if (!m_pTrackFunc) {
        throw std::invalid_argument("mandatory track function is not defined");
    }

    std::cout << "valid modules" << std::endl;


}

void PyTrackingAlgorithm::track(ulong frameNumber, const cv::Mat &frame) {
    PyObject *pFrameNumber = PyLong_FromUnsignedLong(frameNumber);
    PyObject *pMat = convert(frame);
    PyObject *pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, pFrameNumber);
    PyTuple_SetItem(pArgs, 1, pMat);
    PyObject_CallObject(m_pTrackFunc, pArgs);
}

void PyTrackingAlgorithm::paint(ProxyPaintObject &,
                                const TrackingAlgorithm::View &) {

}

void PyTrackingAlgorithm::paintOverlay(QPainter *p) {

}

std::shared_ptr<QWidget> PyTrackingAlgorithm::getToolsWidget() {
    auto ptr = std::make_shared<QWidget>();
    return ptr;
}

std::shared_ptr<QWidget> PyTrackingAlgorithm::getParamsWidget() {
    auto ptr = std::make_shared<QWidget>();
    return ptr;
}

//std::set<Qt::Key> &PyTrackingAlgorithm::grabbedKeys() const {
//    return &m_keys;
//}

void PyTrackingAlgorithm::prepareSave() {

}

void PyTrackingAlgorithm::postLoad() {

}

void PyTrackingAlgorithm::postConnect() {

}

void PyTrackingAlgorithm::mouseMoveEvent(QMouseEvent *) {

}

void PyTrackingAlgorithm::mousePressEvent(QMouseEvent *) {

}

void PyTrackingAlgorithm::mouseWheelEvent(QWheelEvent *) {

}

void PyTrackingAlgorithm::keyPressEvent(QKeyEvent *) {

}

bool PyTrackingAlgorithm::isFunc(PyObject *pFunc) {
    return (pFunc && PyCallable_Check(pFunc));
}

// ==== CONVERTER FOR MAT ====
// taken and inspired from the opencv python code:
// https://github.com/Itseez/opencv/blob/master/modules/python/src2/cv2.cpp
// Converts cv::Mat to PythonObj

class PyAllowThreads {
  public:
    PyAllowThreads() : _state(PyEval_SaveThread()) {}
    ~PyAllowThreads() {
        PyEval_RestoreThread(_state);
    }
  private:
    PyThreadState *_state;
};

class PyEnsureGIL {
  public:
    PyEnsureGIL() : _state(PyGILState_Ensure()) {}
    ~PyEnsureGIL() {
        PyGILState_Release(_state);
    }
  private:
    PyGILState_STATE _state;
};

static PyObject *opencv_error = 0;

#define ERRWRAP2(expr) \
try \
{ \
    PyAllowThreads allowThreads; \
    expr; \
} \
catch (const cv::Exception &e) \
{ \
    PyErr_SetString(opencv_error, e.what()); \
    return 0; \
}

// Dont worry, we "know" what we are doing..
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

static size_t REFCOUNT_OFFSET = (size_t) &(((PyObject *)0)->ob_refcnt) +
                                (0x12345678 != *(const size_t *)"\x78\x56\x34\x12\0\0\0\0\0") *sizeof(int);

static inline PyObject *pyObjectFromRefcount(const int *refcount) {
    return (PyObject *)((size_t)refcount - REFCOUNT_OFFSET);
}

static inline int *refcountFromPyObject(const PyObject *obj) {
    return (int *)((size_t)obj + REFCOUNT_OFFSET);
}


class NumpyAllocator : public cv::MatAllocator {
  public:
    NumpyAllocator() {}
    ~NumpyAllocator() {}

    void allocate(int dims, const int *sizes, int type, int *&refcount,
                  uchar *&datastart, uchar *&data, size_t *step) {
        PyEnsureGIL gil;

        int depth = CV_MAT_DEPTH(type);
        int cn = CV_MAT_CN(type);
        const int f = (int)(sizeof(size_t)/8);
        int typenum = depth == CV_8U ? NPY_UBYTE : depth == CV_8S ? NPY_BYTE :
                      depth == CV_16U ? NPY_USHORT : depth == CV_16S ? NPY_SHORT :
                      depth == CV_32S ? NPY_INT : depth == CV_32F ? NPY_FLOAT :
                      depth == CV_64F ? NPY_DOUBLE : f*NPY_ULONGLONG + (f^1)*NPY_UINT;
        int i;
        npy_intp _sizes[CV_MAX_DIM+1];
        for (i = 0; i < dims; i++) {
            _sizes[i] = sizes[i];
        }
        if (cn > 1) {
            /*if( _sizes[dims-1] == 1 )
                _sizes[dims-1] = cn;
            else*/
            _sizes[dims++] = cn;
        }
        std::cout << "allocate1:" << dims << ":" << typenum << ":" << _sizes << ":" <<
                  CV_MAX_DIM << std::endl;
        PyObject *o = PyArray_SimpleNew(dims, _sizes, typenum);
        if (!o) {
            CV_Error_(CV_StsError,
                      ("The numpy array of typenum=%d, ndims=%d can not be created", typenum, dims));
        }
        std::cout << "allocate2" << std::endl;
        refcount = refcountFromPyObject(o);
        std::cout << "allocate3" << std::endl;
        npy_intp *_strides = PyArray_STRIDES((PyArrayObject *) o);
        for (i = 0; i < dims - (cn > 1); i++) {
            step[i] = (size_t)_strides[i];
        }
        std::cout << "allocate4" << std::endl;
        datastart = data = (uchar *)PyArray_DATA((PyArrayObject *) o);
    }

    void deallocate(int *refcount, uchar *, uchar *) {
        PyEnsureGIL gil;
        if (!refcount) {
            return;
        }
        PyObject *o = pyObjectFromRefcount(refcount);
        Py_INCREF(o);
        Py_DECREF(o);
    }
};

NumpyAllocator g_numpyAllocator;

PyObject *PyTrackingAlgorithm::convert(const cv::Mat &m) {
    if (!m.data) {
        Py_RETURN_NONE;
    }
    cv::Mat temp, *p = (cv::Mat *)&m;
    if (!p->refcount || p->allocator != &g_numpyAllocator) {
        temp.allocator = &g_numpyAllocator;
        ERRWRAP2(m.copyTo(temp));
        Py_RETURN_NONE;
        p = &temp;
    }
    p->addref();
    return pyObjectFromRefcount(p->refcount);
}


#pragma GCC diagnostic pop

// ==== CONVERTER FOR MAT ====

}
}
}
