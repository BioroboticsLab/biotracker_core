#include "../../interpreter/PyInterpreter.h"
#include <Python.h>
#include <thread>
#include <chrono>
#include <opencv2/core/types_c.h>
#include <opencv2/opencv_modules.hpp>

namespace BioTracker {
namespace Core {
namespace Interpreter {

void *init_nu() {
    import_array();
    return NULL;
}

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
PyObject *PyInterpreter::convert(const cv::Mat &m) {
    return PyTrackingAlgorithm::convert(m);
}

}
}
}
