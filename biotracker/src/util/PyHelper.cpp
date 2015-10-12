/*
 * PyHelper.cpp
 * heavily inspired by: https://github.com/Itseez/opencv/blob/master/modules/python/src2/cv2.cpp
 *
 *  Created on: October 6, 2015
 *      Author: Julian
 */

#include "util/PyHelper.h"

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

// Dont worry, we "know" what we are doing.. i guess
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

// magic stuff, rainbows, kittens, etc
static size_t REFCOUNT_OFFSET = (size_t) &(((PyObject *)0)->ob_refcnt) +
                                (0x12345678 != *(const size_t *)"\x78\x56\x34\x12\0\0\0\0\0") *sizeof(int);

static inline PyObject *pyObjectFromRefcount(const int *refcount) {
    return (PyObject *)((size_t)refcount - REFCOUNT_OFFSET);
}

static inline int *refcountFromPyObject(const PyObject *obj) {
    return (int *)((size_t)obj + REFCOUNT_OFFSET);
}

void *init_numpy() {
    import_array();
    return NULL;
}

class NumpyAllocator : public cv::MatAllocator {
  public:
    NumpyAllocator() {}
    ~NumpyAllocator() {}

    void allocate(int dims, const int *sizes, int type, int *&refcount,
                  uchar *&datastart, uchar *&data, size_t *step) {

        PyEnsureGIL gil;
        init_numpy();

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

        PyObject *o = PyArray_SimpleNew(dims, _sizes, typenum);
        if (!o) {
            CV_Error_(CV_StsError,
                      ("The numpy array of typenum=%d, ndims=%d can not be created", typenum, dims));
        }
        refcount = refcountFromPyObject(o);
        npy_intp *_strides = PyArray_STRIDES((PyArrayObject *) o);
        for (i = 0; i < dims - (cn > 1); i++) {
            step[i] = (size_t)_strides[i];
        }
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


PyObject *PyHelper::convert(const cv::Mat &m) {
    if (!m.data) {
        Py_RETURN_NONE;
    }
    cv::Mat temp, *p = (cv::Mat *)&m;
    if (!p->refcount || p->allocator != &g_numpyAllocator) {
        temp.allocator = &g_numpyAllocator;
        ERRWRAP2(m.copyTo(temp));
        p = &temp;
    }
    p->addref();
    return pyObjectFromRefcount(p->refcount);
}

#pragma GCC diagnostic pop

bool PyHelper::isFunc(PyObject *pFunc) {
    return (pFunc && PyCallable_Check(pFunc));
}

std::string PyHelper::errorMessage() {
    PyObject *ptype, *pvalue, *ptrace;
    PyErr_Fetch(&ptype, &pvalue, &ptrace);
    return toString(pvalue);
}

std::string PyHelper::toString(PyObject *o) {
    if (o) {
        if (PyUnicode_Check(o)) {
            char *res = 0;
            PyObject *temp_bytes = PyUnicode_AsEncodedString(o, "ASCII", "strict");
            if (temp_bytes != NULL) {
                res = PyBytes_AS_STRING(temp_bytes);
                res = strdup(res);
                Py_DECREF(temp_bytes);
                std::string result(res);
                return result;
            }
        } else {
            // throw exception
        }
    }
    return "";
}

bool PyHelper::hasError() {
    return PyErr_Occurred() != NULL;
}

void *PyHelper::initNumpy() {
    import_array();
    return NULL;
}

std::string PyHelper::infos() {

    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;

    std::wstring p_w = L"", h_w = L"";
    if (Py_GetPath()) {
        p_w = Py_GetPath();
    }
    if (Py_GetPythonHome()) {
        h_w = Py_GetPythonHome();
    }

    std::string p = converter.to_bytes(p_w);
    std::string h = converter.to_bytes(h_w);
    std::string v = "", pl = "";
    if (Py_GetVersion()) {
        v = Py_GetVersion();
    }
    if (Py_GetPlatform()) {
        pl = Py_GetPlatform();
    }

    std::stringstream ss;

    ss << "path[" << p << "] home:[" << h << "] version:[" << v << "] platform:[" <<
       pl << "]";

    return ss.str();
}
