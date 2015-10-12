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

// =========== C O N V E R T ====================
PyObject *PyInterpreter::convert(const cv::Mat &m) {
    return PyHelper::convert(m);
}

}
}
}
