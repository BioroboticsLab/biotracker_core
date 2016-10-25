#include "TrackingAlgorithm.h"

namespace BioTracker {
namespace Core {


TrackingAlgorithm::TrackingAlgorithm(Settings &settings)
    : QObject()
    , m_settings(settings)
    , m_doTracking(true)
    , m_currentFrameNumber(0)
    , m_currentZoomLevel(0.0f)
    , m_videoMode(GuiParam::VideoMode::Init)
    , m_tools(new QWidget()) {
}

TrackingAlgorithm::~TrackingAlgorithm() {
    if (m_tools) {
        delete m_tools;
    }
}

void TrackingAlgorithm::loadObjects(const std::vector<TrackedObject> &objects) {
    m_trackedObjects = objects;
    postLoad();
}

void TrackingAlgorithm::loadObjects(std::vector<TrackedObject> &&objects) {
    m_trackedObjects = std::move(objects);
    postLoad();
}

const std::vector<TrackedObject> &TrackingAlgorithm::getObjects() {
    prepareSave();
    return m_trackedObjects;
}

QPointer<QWidget> TrackingAlgorithm::getToolsWidget() {
    return m_tools;
}

const std::set<Qt::Key> &TrackingAlgorithm::grabbedKeys() const {
    static const std::set<Qt::Key> keys;
    return keys;
}

bool TrackingAlgorithm::event(QEvent *event) {
    const QEvent::Type etype = event->type();
    switch (etype) {
    case QEvent::KeyPress:
        keyPressEvent(static_cast<QKeyEvent *>(event));
        return true;
    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent *>(event));
        return true;
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent *>(event));
        return true;
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
        return true;
    case QEvent::Wheel:
        mouseWheelEvent(static_cast<QWheelEvent *>(event));
        return true;
    default:
        event->ignore();
        return false;
    }
}

boost::optional<cv::Mat> TrackingAlgorithm::getCurrentImageCopy() const {
    if (m_currentImage) {
        return m_currentImage.get().clone();
    } else {
        return boost::optional<cv::Mat>();
    }
}


const TrackingAlgorithm::View TrackingAlgorithm::OriginalView {"Original"};

}
}
