#include "BioTrackerApp.h"
#include <limits>
#include <thread>

#include <boost/algorithm/string.hpp>

#include "util/QOpenGLContextWrapper.h"
#include "Exceptions.h"

namespace BioTracker {
namespace Core {

BioTrackerApp::BioTrackerApp()
    : m_settings()
    , m_registry(Registry::getInstance())
    , m_trackingThread(m_settings) {
    initConnects();

    loadModulesInPath(CONFIGPARAM::MODULE_PATH);
}

BioTrackerApp::~BioTrackerApp() {
    m_trackingThread.terminate();
    m_trackingThread.wait();
}

void BioTrackerApp::initConnects() {
    QObject::connect(&m_trackingThread, &Core::TrackingThread::frameCalculated,
                     this, &BioTrackerApp::frameCalculatedFromTrackingThread);
    QObject::connect(&m_trackingThread, &Core::TrackingThread::notifyGUI,
                     this, &BioTrackerApp::notifyFromTrackingThread);
    QObject::connect(&m_trackingThread, &Core::TrackingThread::fileOpened,
                     this, &BioTrackerApp::fileOpenedFromTrackingThread);
}

void BioTrackerApp::initializeOpenGL(QOpenGLContext *mainContext,
                                     TextureObject &texture) {
    m_trackingThread.initializeOpenGL(mainContext, texture);
}

void BioTrackerApp::openVideo(const boost::filesystem::path &path) {
    m_trackingThread.loadVideo(path);
}

void BioTrackerApp::openImages(std::vector<boost::filesystem::path> paths) {
    m_trackingThread.loadPictures(std::move(paths));
}

void BioTrackerApp::openCamera(int device) {
    m_trackingThread.openCamera(device);
}

void BioTrackerApp::play() {
    m_trackingThread.setPlay();
}

void BioTrackerApp::pause() {
    m_trackingThread.setPause();
}

void BioTrackerApp::paint(QPaintDevice &device, QPainter &painter) {
    m_trackingThread.paint(device, painter);
}

bool BioTrackerApp::isRendering() {
    return m_trackingThread.isRendering();
}

bool BioTrackerApp::isRunning() {
    return !m_trackingThread.isPaused();
}

void BioTrackerApp::setFrame(const size_t frameNumber) {
    m_trackingThread.setFrameNumber(frameNumber);
}

void BioTrackerApp::setTargetFps(const double fps) {
    m_trackingThread.setFps(fps);
}

void BioTrackerApp::setMaxSpeed(bool enabled) {
    m_trackingThread.setMaxSpeed(enabled);
}

double BioTrackerApp::getTargetFps() const {
    return std::numeric_limits<double>::infinity();
}

size_t BioTrackerApp::getNumFrames() const {
    return m_trackingThread.getVideoLength();
}

size_t BioTrackerApp::getCurrentFrameNumber() const {
    return m_trackingThread.getFrameNumber();
}

void BioTrackerApp::setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm>
        trackingAlgorithm) {
    m_trackingThread.setTrackingAlgorithm(std::move(trackingAlgorithm));
}

void BioTrackerApp::mouseEvent(QMouseEvent *event) {
    m_trackingThread.mouseEvent(event);
}

void BioTrackerApp::keyboardEvent(QKeyEvent *event) {
    m_trackingThread.keyboardEvent(event);
}

// all slots will only pass the signals through

void BioTrackerApp::notifyFromTrackingThread(const std::string &message,
        const MSGS::MTYPE type) {
    Q_EMIT notify(message, type);
}

/**
 * @brief BioTrackerApp::frameCalculatedFromTrackingThread
 * @param frameNumber the number of the last frame that was calculated
 * @param filename
 * @param currentFps, when -1, then the stream is not playing but the user clicked next
 */
void BioTrackerApp::frameCalculatedFromTrackingThread(const size_t frameNumber,
        const std::string filename, const double currentFps) {
    Q_EMIT frameCalculated(frameNumber, filename, currentFps);
}

void BioTrackerApp::loadModulesInPath(const boost::filesystem::path &path) {
    if (!boost::filesystem::is_directory(path)) {
        throw directory_not_found("Invalid path");
    }

    boost::filesystem::directory_iterator directoryIt(path);
    for (const boost::filesystem::path &p : directoryIt) {
        std::vector<std::string> parts;
        boost::split(parts, p.string(), boost::is_any_of("."));
        /* expect filename to be of form: name.tracker.ext */
        if (parts.size() >= 3) {
            getRegistry().loadTrackerLibrary(p);
        }
    }
}

void BioTrackerApp::fileOpenedFromTrackingThread(const std::string fileName,
        const size_t numFrame, const double tfps) {
    Q_EMIT fileOpened(fileName, numFrame, tfps);
}

} // Core
} // BioTracker
