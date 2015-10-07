#pragma once
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <QObject>
#include <QDir>
#include <QString>
#include <QStringList>
#include <iostream>
#include <QFile>
#include <QHash>
#include <stdlib.h>
#include <QWidget>
#include <boost/filesystem/path.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <thread>
#include <numpy/ndarrayobject.h>
#include "PyTrackingAlgorithm.h"
#include "../settings/Settings.h"
#include "../util/PyHelper.h"

namespace BioTracker {
namespace Core {
namespace Interpreter {

//TODO make this a singleton as we must initialize the py interpreter ONLY once!
/**
 * @brief The PyInterpreter class
 */
class PyInterpreter : public QObject {
  public:
    Q_OBJECT
  public:
    PyInterpreter() {
        //PyEval_InitThreads();
        std::wstring name = L"biotrack";
        wchar_t *n =  const_cast<wchar_t *>(name.c_str());
        Py_SetProgramName(n);
        init_numpy();

        npy_intp length[1];
        length[0] = 10;
        PyObject *my_array = PyArray_SimpleNew(1, length, NPY_FLOAT);
        npy_intp length1[1];
        length1[0] = 10;
        PyObject *my_array1 = PyArray_SimpleNew(1, length1, NPY_FLOAT);


        // import standard libs
        PyRun_SimpleString("import sys");



    }
    ~PyInterpreter() {
        Py_Finalize();
    }

    void *init_numpy() const {
        std::cout << "before init numpy" << std::this_thread::get_id() << std::endl;
        Py_Initialize();
        import_array();
        std::cout << "init numpy" << std::endl;
        return NULL;
    }

    /**
     * @brief loadScript
     * @param path
     * @return Name of the script
     */
    std::string loadScript(const boost::filesystem::path &path);

    /**
     * @brief hasModule
     * @param name
     * @return True if the module is loaded as python module, otherwise false
     */
    bool hasModule(const std::string &name) const;

    /**
     * @brief activatePythonModule
     * sets the given module as the current module
     * @param script string that was created by {loadScript}
     */
    std::shared_ptr<PyTrackingAlgorithm> activatePythonModule(
        const std::string &script, Settings &s, QWidget *p);

    QStringList getLoadedModules() const;

    /**
     * @brief isFunc
     * @param pFunc
     * @return True if the object is a function, false otherwise
     */
    bool isFunc(PyObject *pFunc);

    /**
     * @brief convert
     * Converts a cv::Mat to its python equivalent
     * @param m
     * @return
     */
    PyObject *convert(const cv::Mat &m);

  Q_SIGNALS:
    /**
     * @brief moduleAlreadyLoaded
     * @param moduleName
     */
    void moduleIsAlreadyLoaded(const std::string &moduleName);

    /**
     * @brief moduleIsNotLoaded
     * gets called when an attempt to activate a module is done which
     * is not loaded yet
     * @param moduleName
     */
    void moduleIsNotLoaded(const std::string &moduleName);

  private:

    /**
     * @brief addToPyPath
     * Adds the path where the file resides to the sys.path, if it
     * is not added yet!
     * @param path, full path to the python file
     */
    void addToPyPath(const boost::filesystem::path &path);

    // TODO change this to sets..
    /**
     * @brief m_importedPaths
     * remembers which paths are already imported so that we do not import
     * them multiple times
     */
    QStringList m_importedPaths;

    /**
     * @brief m_importedModules
     * remembers modules that are imported
     */
    QStringList m_importedModules;

    /**
     * @brief m_loadedModules
     * modules that are actually loaded
     */
    QHash<QString, std::shared_ptr<PyTrackingAlgorithm>> m_loadedModules;

    /**
     * @brief currentModule
     */
    PyObject *m_currentModule;
};

}
}
}
