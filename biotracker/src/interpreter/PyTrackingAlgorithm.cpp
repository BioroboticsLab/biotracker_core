#include "../../interpreter/PyTrackingAlgorithm.h"

namespace BioTracker {
namespace Core {
namespace Interpreter {

PyTrackingAlgorithm::PyTrackingAlgorithm(QString moduleName, Settings &s,
        QWidget *p) :
    TrackingAlgorithm(s, p),
    m_pModule(PyImport_ImportModule(moduleName.toStdString().data())) {
    m_pTrackFunc = PyObject_GetAttrString(m_pModule, "track");
    if (!m_pTrackFunc) {
        throw std::invalid_argument("mandatory track function is not defined");
    }
    Py_INCREF(m_pTrackFunc);

    // this functions are optional..
    m_pPaintOverlayFunc = PyObject_GetAttrString(m_pModule, "paintOverlay");
}

void PyTrackingAlgorithm::track(ulong frameNumber, const cv::Mat &frame) {
    PyObject *pMat = convert(frame);
    PyObject *pFrameNumber = PyLong_FromUnsignedLong(frameNumber);
    PyObject *pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, pFrameNumber);
    PyTuple_SetItem(pArgs, 1, pMat);
    PyObject *o = PyObject_CallObject(m_pTrackFunc, pArgs);
    PyObject *str = PyObject_Repr(o);
    const char *txt = PyUnicode_AsUTF8(str);
}

void PyTrackingAlgorithm::paint(ProxyPaintObject &,
                                const TrackingAlgorithm::View &) {

}

void PyTrackingAlgorithm::paintOverlay(QPainter *p) {
    if (PyHelper::isFunc(m_pPaintOverlayFunc)) {
        // TODO: more stuff.. like convert the QPainter
        PyObject *pArgs = PyTuple_New(0);
        PyObject_CallObject(m_pPaintOverlayFunc, pArgs);
    }
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

PyObject *PyTrackingAlgorithm::convert(const cv::Mat &m) {
    return PyHelper::convert(m);
}


}
}
}
