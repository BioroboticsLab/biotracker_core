#include "TrackingThread.h"

#include <iostream>

#include <chrono>
#include <thread>

#include <QFileInfo>

#include "settings/Messages.h"
#include "settings/Settings.h"
#include "settings/ParamNames.h"

#include <QCoreApplication>
#include <QtOpenGL/qgl.h>

#include <biotracker/util/ScreenHelper.h>
#include <QPainter>

namespace BioTracker {
namespace Core {

using GUIPARAM::MediaType;

TrackingThread::TrackingThread(Settings &settings) :
    m_imageStream(make_ImageStreamNoMedia()),
    m_playing(false),
    m_playOnce(false),
    m_somethingIsLoaded(false),
    m_status(TrackerStatus::NothingLoaded),
    m_fps(30),
    m_runningFps(0),
    m_maxSpeed(false),
    m_mediaType(MediaType::NoMedia),
    m_settings(settings),
    m_texture(nullptr) {
    Interpreter::Interpreter p;
}

TrackingThread::~TrackingThread(void) {
}

void TrackingThread::initializeOpenGL(QOpenGLContext *context,
                                      TextureObject &texture) {
    m_texture = &texture;
    QThread::start();
}

void TrackingThread::loadFromSettings() {
    std::string filenameStr = m_settings.getValueOfParam<std::string>
                              (CAPTUREPARAM::CAP_VIDEO_FILE);
    boost::filesystem::path filename {filenameStr};
    m_imageStream = make_ImageStreamVideo(filename);
    if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open file " + filename.string();
        Q_EMIT notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    } else {
        playOnce();
    }

    m_fps = m_imageStream->fps();

    Q_EMIT fileOpened(filenameStr, m_imageStream->numFrames(), m_fps);

    std::string note = "opened file: " + filenameStr + " (#frames: "
                       + QString::number(m_imageStream->numFrames()).toStdString() + ")";
    Q_EMIT notifyGUI(note, MSGS::MTYPE::FILE_OPEN);
}

void TrackingThread::loadVideo(const boost::filesystem::path &filename) {
    m_imageStream = make_ImageStreamVideo(filename);
    if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open file " + filename.string();
        Q_EMIT notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    } else {
        playOnce();
    }

    m_fps = m_imageStream->fps();

    m_settings.setParam(CAPTUREPARAM::CAP_VIDEO_FILE, filename.string());

    std::string note = filename.string() + " (#frames: "
                       + QString::number(m_imageStream->numFrames()).toStdString() + ")";
    Q_EMIT fileOpened(filename.string(), m_imageStream->numFrames(), m_fps);
    Q_EMIT notifyGUI(note, MSGS::MTYPE::FILE_OPEN);
}

void TrackingThread::loadPictures(std::vector<boost::filesystem::path>
                                  &&filenames) {
    m_fps = 1;
    m_imageStream = make_ImageStreamPictures(std::move(filenames));
    if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open files [";
        for (boost::filesystem::path filename: filenames) {
            errorMsg += ", " + filename.string();
        }
        errorMsg += "]";
        Q_EMIT notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    } else {
        playOnce();
        Q_EMIT fileOpened(m_imageStream->currentFilename(), m_imageStream->numFrames(),
                          m_fps);
    }
}

void TrackingThread::openCamera(int device) {
    m_imageStream = make_ImageStreamCamera(device);
    if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open camera " + QString::number(
                                   device).toStdString();
        Q_EMIT notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    }
    m_status = TrackerStatus::Running;
    m_fps = m_imageStream->fps();
    std::string note = "open camera " + QString::number(device).toStdString();
    Q_EMIT notifyGUI(note, MSGS::MTYPE::NOTIFICATION);
    m_somethingIsLoaded = true;
}

void TrackingThread::run() {
    std::chrono::system_clock::time_point t;
    bool firstLoop = true;

    while (true) {
        std::unique_lock<std::mutex> lk(m_tickMutex);


        m_conditionVariable.wait(lk, [&] {return (m_playing || m_playOnce) && !m_isRendering;});
        m_isRendering = true;

        m_playOnce = false;
        if (m_imageStream->lastFrame()) {
            setPause();
        }


        //if thread just started (or is unpaused) start clock here
        //after this timestamp will be taken right before picture is drawn
        //to take the amount of time into account it takes to draw the picture
        if (firstLoop) { // measure the capture start time
            t = std::chrono::system_clock::now();
        }
        firstLoop = false;

        std::chrono::microseconds target_dur(static_cast<int>(1000000. / m_fps));
        std::chrono::microseconds dur =
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now() - t);
        if (!m_maxSpeed) {
            if (dur <= target_dur) {
                target_dur -= dur;
            } else {
                target_dur = std::chrono::microseconds(0);
            }
        } else {
            target_dur = std::chrono::microseconds(0);
        }


        // calculate the running fps.
        // TODO: why is this a member var??
        m_runningFps = 1000000. / std::chrono::duration_cast<std::chrono::microseconds>
                       (dur + target_dur).count();

        if (m_playOnce && !m_playing) {
            m_runningFps = -1;
        }

        tick(m_runningFps);

        std::this_thread::sleep_for(target_dur);
        t = std::chrono::system_clock::now();

        lk.unlock();
    }
}

void TrackingThread::tick(const double fps) {
    m_renderMutex.lock();
    std::string fileName = m_imageStream->currentFilename();
    doTracking();
    const size_t currentFrame = m_imageStream->currentFrameNumber();
    if (m_playing) {
        nextFrame();
    }
    m_renderMutex.unlock();
    m_paintMutex.lock();
    Q_EMIT frameCalculated(currentFrame, fileName, fps);
    m_paintMutex.unlock();
}

void TrackingThread::setFrameNumber(size_t frameNumber) {
    m_renderMutex.lock();
    if (m_imageStream->setFrameNumber(frameNumber)) {
        if (m_tracker) {
            m_tracker->setCurrentFrameNumber(frameNumber);
        }
        playOnce();
    }
    m_renderMutex.unlock();
}
void TrackingThread::nextFrame() {
    if (m_imageStream->nextFrame()) { // increments the frame number if possible
        if (m_tracker) {
            m_tracker->setCurrentFrameNumber(m_imageStream->currentFrameNumber());
        }
    } else {
        m_playing = false;
        m_playOnce = false;
    }
}

void TrackingThread::doTracking() {
    MutexLocker trackerLock(m_trackerMutex);
    if (!m_tracker) {
        return;
    }

    // do nothing if we aint got a frame
    if (m_imageStream->currentFrameIsEmpty()) {
        return;
    }
    try {
        m_tracker->track(m_imageStream->currentFrameNumber(),
                         m_imageStream->currentFrame());
    } catch (const std::exception &err) {
        Q_EMIT notifyGUI("critical error in selected tracking algorithm: " +
                         std::string(
                             err.what()), MSGS::FAIL);
    }
}

size_t TrackingThread::getVideoLength() const {
    return m_imageStream->numFrames();
}

void TrackingThread::mouseEvent(QMouseEvent *event) {
    MutexLocker lock(m_trackerMutex);
    if (m_tracker) {
        QCoreApplication::sendEvent(m_tracker.get(), event);
    }
}

void BioTracker::Core::TrackingThread::mouseWheelEvent(QWheelEvent *event) {
    MutexLocker lock(m_trackerMutex);
    if (m_tracker) {
        QCoreApplication::sendEvent(m_tracker.get(), event);
    }
}

void TrackingThread::keyboardEvent(QKeyEvent *event) {
    MutexLocker lock(m_trackerMutex);
    if (m_tracker) {
        QCoreApplication::sendEvent(m_tracker.get(), event);
    }
}

size_t TrackingThread::getFrameNumber() const {
    return m_imageStream->currentFrameNumber();
}

void TrackingThread::setPause() {
    m_playing = false;
    m_status = TrackerStatus::Paused;
}

void TrackingThread::setPlay() {
    m_playing = true;
    m_status = TrackerStatus::Running;
    m_conditionVariable.notify_all();
}

void TrackingThread::paintDone() {
    if (m_somethingIsLoaded) {
        m_isRendering = false;
        m_conditionVariable.notify_all();
    }
}

void TrackingThread::togglePlaying() {
    if (m_playing) {
        setPause();
    } else {
        setPlay();
    }
}

void TrackingThread::playOnce() {
    m_status = TrackerStatus::Paused;
    m_playOnce = true;
    m_somethingIsLoaded = true;
    m_conditionVariable.notify_all();
}

bool TrackingThread::isPaused() const {
    return !m_playing;
}

bool TrackingThread::isRendering() const {
    return m_isRendering;
}

double TrackingThread::getFps() const {
    return m_fps;
}

void TrackingThread::setFps(double fps) {
    m_fps = fps;
}

void TrackingThread::setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm>
        trackingAlgorithm) {
    {
        MutexLocker lock(m_trackerMutex);

        if (m_tracker) {
            m_tracker.get()->disconnect();
        }

        m_tracker = trackingAlgorithm;
        // connect the tracker
        QObject::connect(m_tracker.get(), &TrackingAlgorithm::registerViews,
                         this, &TrackingThread::registerViewsFromTracker);
        QObject::connect(m_tracker.get(), &TrackingAlgorithm::update,
                         this, &TrackingThread::requestPaint);
        QObject::connect(m_tracker.get(), &TrackingAlgorithm::forceTracking,
                         this, &TrackingThread::requestTrackFromTracker);
        QObject::connect(m_tracker.get(), &TrackingAlgorithm::notifyGUI,
                         this, &TrackingThread::notifyGUIFromTracker);
        m_tracker.get()->postConnect();
    }
    Q_EMIT trackerSelected(trackingAlgorithm);

}

void TrackingThread::setMaxSpeed(bool enabled) {
    m_maxSpeed = enabled;
}

void BioTracker::Core::TrackingThread::paint(const size_t w, const size_t h, QPainter &painter,
        BioTracker::Core::PanZoomState &zoom, TrackingAlgorithm::View const &v) {

    // clear background
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRect(QRect(0, 0, w, h));
    painter.setBrush(QColor(0, 0, 0, 0));

    m_paintMutex.lock();
    // using painters algorithm to draw in the right order
    if (m_somethingIsLoaded) {
        cv::Mat m = m_imageStream->currentFrame().clone();
        if (m_tracker) {
            m_tracker.get()->paint(m, v);
        }

        //m_texture->setImage(m);
        const QImage matImg = m_texture->gen(m);


        // We use setWindow and setViewport to fit the video into the
        // given video widget frame (with width "w" and height "h")
        // we later need to adjust an offset caused the use of different
        // dimensions for window and viewport.

        // adjust the panning as the viewport is potentially scewed
        // and mouse movements given by the window are not translated
        // one-to-one anymore
        QRect window;
        QRect viewport;
        const float viewport_scew = ScreenHelper::calculate_viewport(
                                        matImg.width(), matImg.height(), w, h, window, viewport
                                    );

        painter.setWindow(window);
        painter.setViewport(viewport);

        float zoomFactor = 1 + zoom.zoomFactor;

        painter.scale(zoomFactor, zoomFactor);

        painter.translate(
            QPointF(
                -zoom.panX * viewport_scew / zoomFactor,
                -zoom.panY * viewport_scew / zoomFactor
            )
        );

        // TODO only paint that part that is visible
        painter.drawImage(
            QRectF(0, 0 , matImg.width(), matImg.height()),
            matImg,
            QRect(0, 0, matImg.width(), matImg.height()));

        painter.setPen(QColor(255, 0, 0));
        painter.drawText(50, 50, QString("Memel"));

        if (m_tracker) {
            m_tracker.get()->paintOverlay(&painter, v);
        }

        paintDone();
    }
    m_paintMutex.unlock();
}

void BioTracker::Core::TrackingThread::registerViewsFromTracker(const std::vector<TrackingAlgorithm::View> views) {
    Q_EMIT registerViews(views);
}

void BioTracker::Core::TrackingThread::requestPaintFromTracker() {
    Q_EMIT requestPaint();
}

void BioTracker::Core::TrackingThread::requestTrackFromTracker() {
    playOnce();
}

void BioTracker::Core::TrackingThread::notifyGUIFromTracker(std::string m, MSGS::MTYPE type) {
    Q_EMIT notifyGUI(m, type);
}


}
}
