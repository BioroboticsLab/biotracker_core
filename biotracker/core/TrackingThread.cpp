#include "TrackingThread.h"

#include <iostream>

#include <chrono>
#include <thread>

#include <QFileInfo>

#include "biotracker/core/settings/Messages.h"
#include "biotracker/core/settings/Settings.h"
#include "biotracker/core/settings/ParamNames.h"

#include <QCoreApplication>
#include <QtOpenGL/qgl.h>

namespace BioTracker {
namespace Core {

using GUIPARAM::MediaType;

TrackingThread::TrackingThread(Settings &settings) :
    m_imageStream(make_ImageStreamNoMedia()),
    m_playing(false),
    m_playOnce(false),
    m_status(TrackerStatus::NothingLoaded),
    m_fps(30),
    m_runningFps(0),
    m_maxSpeed(false),
    m_mediaType(MediaType::NoMedia),
    m_settings(settings),
    m_texture(nullptr),
    m_openGLLogger(this) {
}

TrackingThread::~TrackingThread(void) {
}

void TrackingThread::initializeOpenGL(std::unique_ptr<Util::SharedOpenGLContext>
                                      &&context, TextureObject &texture) {
    m_context = std::move(context);
    m_texture = &texture;
    m_surface.setFormat(m_context->format());
    m_surface.create();

    m_openGLLogger.initialize(); // initializes in the current context, i.e. ctx
    connect(&m_openGLLogger, &QOpenGLDebugLogger::messageLogged, this,
            &TrackingThread::handleLoggedMessage);
    m_openGLLogger.startLogging();

    QThread::start();
}

void TrackingThread::loadVideo(const boost::filesystem::path &filename) {
    m_imageStream = make_ImageStreamVideo(filename);
    if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open file " + filename.string();
        emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    } else {
        playOnce();
    }

    m_fps = m_imageStream->fps();

    // TODO: crashes at getValueOfParam
    std::string note = "abc";
//        std::string note = "open file: " + m_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE) +
//            " (#frames: " + QString::number(m_imageStream->numFrames()).toStdString() + ")";
    emit notifyGUI(note, MSGS::MTYPE::FILE_NAME_CHANGE);
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
        emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    } else {
        playOnce();
    }
}

void TrackingThread::openCamera(int device) {
    m_imageStream = make_ImageStreamCamera(device);
    if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open camera " + QString::number(
                                   device).toStdString();
        emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    }
    m_status = TrackerStatus::Running;
    m_fps = m_imageStream->fps();
    std::string note = "open camera " + QString::number(device).toStdString();
    emit notifyGUI(note, MSGS::MTYPE::NOTIFICATION);
}

void TrackingThread::run() {
    std::chrono::system_clock::time_point t;
    bool firstLoop = true;

    while (true) {
        std::unique_lock<std::mutex> lk(m_tickMutex);
        m_conditionVariable.wait(lk, [&] {return m_playing || m_playOnce;});
        m_playOnce = false;


        //if thread just started (or is unpaused) start clock here
        //after this timestamp will be taken right before picture is drawn
        //to take the amount of time into account it takes to draw the picture
        if (firstLoop)
            // measure the capture start time
        {
            t = std::chrono::system_clock::now();
        }
        firstLoop = false;

        if ((m_imageStream->type() == GUIPARAM::MediaType::Video)
                && m_imageStream->lastFrame()) {
            break;
        }

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

        tick();

        //std::this_thread::sleep_for(target_dur);
        t = std::chrono::system_clock::now();

        // unlock mutex
        lk.unlock();
    }
}

void TrackingThread::tick() {
    m_context->makeCurrent(&m_surface);

    m_texture->setImage(m_imageStream->currentFrame().clone());

    std::string fileName = m_imageStream->currentFilename();
    double fps = m_imageStream->fps();

    //emit frameCalculated(m_imageStream->currentFrameNumber(), "kurukurukuru", fps);
    doTracking();
    emit frameCalculated(m_imageStream->currentFrameNumber(), fileName, fps);

    if (m_playing) {
        nextFrame();
    }
    m_context->doneCurrent();
}

void TrackingThread::setFrameNumber(size_t frameNumber) {
    if (m_imageStream->setFrameNumber(frameNumber)) {
        if (m_tracker) {
            m_tracker->setCurrentFrameNumber(frameNumber);
        }
        playOnce();
    }
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
        emit notifyGUI("critical error in selected tracking algorithm: " + std::string(
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
    m_conditionVariable.notify_all();
}

bool TrackingThread::isPaused() const {
    return !m_playing;
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
        m_tracker = trackingAlgorithm;
    }
}

void TrackingThread::setMaxSpeed(bool enabled) {
    m_maxSpeed = enabled;
}

void TrackingThread::handleLoggedMessage(const QOpenGLDebugMessage
        &debugMessage) {
    std::cout << debugMessage.message().toStdString() << std::endl;
}

}
}
