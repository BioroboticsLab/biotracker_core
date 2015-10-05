#include "../../interpreter/PyInterpreter.h"
#include <Python.h>
#include <thread>
#include <chrono>
#include <opencv2/core/types_c.h>
#include <opencv2/opencv_modules.hpp>

namespace BioTracker {
namespace Core {
namespace Interpreter {

std::string BioTracker::Core::Interpreter::PyInterpreter::loadScript(
    const boost::filesystem::path &path) {
    const auto moduleName = QString(path.stem().string().data());
    if (m_importedModules.contains(moduleName)) {
        Q_EMIT moduleIsAlreadyLoaded(moduleName.toStdString());
    } else {
        addToPyPath(path);
        m_importedModules.append(moduleName);
    }
    return moduleName.toStdString();

    /*
    QString newpath = QDir::currentPath() + "/" + QString(path.filename().string().data());
    QString oldPath = QString(path.string().data());
    QFile::copy(oldPath, newpath);
    const char* importModule = path.stem().string().data();

    QString importModuleStr = QString(importModule);
    wchar_t* a = new wchar_t[importModuleStr.length() + 1];
    importModuleStr.toWCharArray(a);
    a[importModuleStr.length()] = '\0';

    Py_SetProgramName(a);

    const char* pyDirectory = path.parent_path().string().data();
    const size_t cSize = strlen(pyDirectory) + 1;
    wchar_t* pyDir = new wchar_t[cSize];
    auto pyDirIntermediate = QString(pyDirectory);
    pyDirIntermediate.toWCharArray(pyDir);
    pyDir[cSize-1] = '\0';

    //Py_SetPythonHome(pyDir);
    std::wcout << pyDir << std::endl;

    PySys_SetPath(pyDir);

    PyRun_SimpleString("import sys\nsys.path.append('')\nprint('set sys..')");



    PyRun_SimpleString("import example_tracker\nprint('import example tracker')");

    PyObject *pName = PyUnicode_FromString(importModule);

    if (pName) {
        std::cout << "cool" << std::endl;
    }

    PyObject *pModule = PyImport_Import(pName);

    Py_DECREF(pName);
    if (pModule) {

        std::cout << "good" << std::endl;

    } else {
        std::cout << "fuck" << " in import " << importModule << std::endl;
        PyErr_Print();
        std::exit(1);
    }
    */




}

bool PyInterpreter::hasModule(const std::__cxx11::string &name) const {
    const auto temp = QString(name.data());
    return m_importedModules.contains(temp);
}

std::shared_ptr<PyTrackingAlgorithm> PyInterpreter::activatePythonModule(
    const std::string &script, Settings &s, QWidget *p) {
    const QString temp = QString(script.data());
    if (m_importedModules.contains(temp)) {
        if (!m_loadedModules.contains(temp)) {
            auto tracker = std::make_shared<PyTrackingAlgorithm>(temp, s, p);
            m_loadedModules.insert(temp, tracker);
        }

        return m_loadedModules.take(temp);
    } else {
        Q_EMIT moduleIsNotLoaded(script);
        throw std::invalid_argument("nope.."); // TODO make this better..
    }
}

QStringList PyInterpreter::getLoadedModules() const {
    return m_importedModules;
}

void PyInterpreter::addToPyPath(const boost::filesystem::path &path) {
    auto pathAsQStr = QString(path.parent_path().string().data());
    if (!m_importedPaths.contains(pathAsQStr)) {
        m_importedPaths.append(pathAsQStr);
        pathAsQStr.prepend("sys.path.append('");
        pathAsQStr.append("')");
        PyRun_SimpleString(pathAsQStr.toUtf8().data());
    }
}

bool PyInterpreter::isFunc(PyObject *pFunc) {
    return (pFunc && PyCallable_Check(pFunc));
}

// =========== C O N V E R T ====================
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

PyObject *PyInterpreter::convert(const cv::Mat &m) {
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

}
}
}
