#include "TrackingThread.h"

#include <iostream>
#include <sstream>

#include <chrono>
#include <thread>

#include <QFileInfo>

#include "settings/Messages.h"
#include "settings/Settings.h"
#include "settings/ParamNames.h"
#include "Exceptions.h"

#include <QCoreApplication>
#include <QtOpenGL/qgl.h>

#include <biotracker/util/ScreenHelper.h>
#include <QPainter>

namespace BioTracker {
namespace Core {

using GuiParam::MediaType;

TrackingThread::TrackingThread(Settings &settings) :
    m_imageStream(make_ImageStreamNoMedia()),
    m_playing(false),
    m_playOnce(false),
    m_somethingIsLoaded(false),
    m_isTrackingEnabled(true),
    m_status(TrackerStatus::NothingLoaded),
    m_fps(30),
    m_maxSpeed(false),
    m_mediaType(MediaType::NoMedia),
    m_settings(settings) {
    Interpreter::Interpreter p;
}

TrackingThread::~TrackingThread(void) {
}

void TrackingThread::loadFromSettings() {
    // Determine which media type was used the last time
    boost::optional<uint8_t> mediaTypeOpt = m_settings.maybeGetValueOfParam<uint8_t>(GuiParam::MEDIA_TYPE);
    GuiParam::MediaType mediaType = mediaTypeOpt ? static_cast<GuiParam::MediaType>(*mediaTypeOpt) :
                                    GuiParam::MediaType::NoMedia;

    if (mediaType == GuiParam::MediaType::Video) {
        boost::optional<std::string> filenameStr = m_settings.maybeGetValueOfParam<std::string>(CaptureParam::CAP_VIDEO_FILE);
        // Abort, because filename string was not set (just to prevent manipulated config files)
        if (!filenameStr) {
            m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(GuiParam::MediaType::NoMedia));
            return;
        }
        boost::filesystem::path filename {*filenameStr};
        try {
            loadVideo(filename);
        } catch (file_not_found &e) {
            // Preventing segfault when video file vanished
            m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(GuiParam::MediaType::NoMedia));
            Q_EMIT notifyGUI(e.what(), MessageType::FAIL);
        }
    } else if (mediaType == GuiParam::MediaType::Camera) {
        boost::optional<int> camIdOpt = m_settings.maybeGetValueOfParam<int>(CaptureParam::CAP_CAMERA_ID);
        int camId = camIdOpt ? *camIdOpt : -1;
        try {
            loadCamera(camId);
        } catch (device_open_error e) {
            m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(GuiParam::MediaType::NoMedia));
            Q_EMIT notifyGUI(e.what(), MessageType::FAIL);
        }
    } else if (mediaType == GuiParam::MediaType::Images) {
        boost::optional<std::string> filenamesStrOpt = m_settings.maybeGetValueOfParam<std::string>
                (PictureParam::PICTURE_FILES);

        if (!filenamesStrOpt) {
            m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(GuiParam::MediaType::NoMedia));
            return;
        }
        std::string filenamesStr = *filenamesStrOpt;

        // Split string of paths into a vector a paths
        // source: http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
        std::vector<boost::filesystem::path> filenames;
        std::string delimiter = ";";
        size_t pos = 0;
        std::string token;
        while ((pos = filenamesStr.find(delimiter)) != std::string::npos) {
            token = filenamesStr.substr(0, pos);
            filenames.push_back(boost::filesystem::path(token));
            filenamesStr.erase(0, pos + delimiter.length());
        }
        try {
            loadPictures(std::move(filenames));
        } catch (file_not_found e) {
            m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(GuiParam::MediaType::NoMedia));
            Q_EMIT notifyGUI(e.what(), MessageType::FAIL);
        }
    } else {
        m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(GuiParam::MediaType::NoMedia));
    }

}

void TrackingThread::loadVideo(const boost::filesystem::path &filename) {
    m_imageStream = make_ImageStreamVideo(filename);
    if (m_imageStream->type() == GuiParam::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open file " + filename.string();
        Q_EMIT notifyGUI(errorMsg, MessageType::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    } else {
        m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(m_imageStream->type()));
        playOnce();
    }

    m_fps = m_imageStream->fps();
    m_ignoreFilenameChanged = false;

    m_settings.setParam(CaptureParam::CAP_VIDEO_FILE, filename.string());

    std::string note = filename.string() + " (#frames: "
                       + QString::number(m_imageStream->numFrames()).toStdString() + ")";
    Q_EMIT fileOpened(filename.string(), m_imageStream->numFrames(), m_fps);
    if (m_tracker) {
        m_tracker->inputChanged();
        if (m_somethingIsLoaded && m_lastFilename.compare(filename.string()) != 0) {
            m_tracker->onFileChanged(filename.string());
            m_lastFilename = filename.string();
        }
    }
    Q_EMIT notifyGUI(note, MessageType::FILE_OPEN);
}

void TrackingThread::loadPictures(std::vector<boost::filesystem::path>
                                  &&filenames) {
    m_fps = 1;
    m_ignoreFilenameChanged = false;

    // Convert filenames into one string for settings. This is done here, because move may clear the vector with filenames.
    std::stringstream filenamesStr;
    if (!filenames.empty()) {
        filenamesStr << filenames[0].string();
        for (uint32_t i = 1; i < filenames.size(); i++) {
            filenamesStr << ';';
            filenamesStr << filenames[i].string();
        }
    }

    m_imageStream = make_ImageStreamPictures(std::move(filenames));
    if (m_imageStream->type() == GuiParam::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open files [";
        for (boost::filesystem::path filename: filenames) {
            errorMsg += ", " + filename.string();
        }
        errorMsg += "]";
        Q_EMIT notifyGUI(errorMsg, MessageType::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    } else {
        m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(m_imageStream->type()));
        playOnce();
        Q_EMIT fileOpened(m_imageStream->currentFilename(), m_imageStream->numFrames(),
                          m_fps);

        m_settings.setParam(PictureParam::PICTURE_FILES, filenamesStr.str());
        if (m_tracker) {
            m_tracker->inputChanged();
            if (m_somethingIsLoaded
                    && m_lastFilename.compare(m_imageStream->currentFilename()) != 0) {
                m_tracker->onFileChanged(m_imageStream->currentFilename());
                m_lastFilename = m_imageStream->currentFilename();
            }
        }
    }
}

void TrackingThread::loadCamera(int device) {
    // Need to clear _imageStream var because reopening the same camera again will result
    // in an error otherwise (e.g. when you press stop and play again)
    if (m_imageStream->type() == MediaType::Camera) {
        m_imageStream = make_ImageStreamNoMedia();
    }
    m_imageStream = make_ImageStreamCamera(device);
    if (m_imageStream->type() == GuiParam::MediaType::NoMedia) {
        // could not open video
        std::string errorMsg = "unable to open camera " + QString::number(device).toStdString();
        Q_EMIT notifyGUI(errorMsg, MessageType::FAIL);
        m_status = TrackerStatus::Invalid;
        return;
    }

    m_fps = m_imageStream->fps();
    m_settings.setParam(CaptureParam::CAP_CAMERA_ID, device);
    m_settings.setParam<uint8_t>(GuiParam::MEDIA_TYPE, static_cast<uint8_t>(m_imageStream->type()));
    std::string note = "open camera " + QString::number(device).toStdString();
    Q_EMIT notifyGUI(note, MessageType::NOTIFICATION);

    playOnce();
    std::string filename = m_imageStream->currentFilename();
    Q_EMIT fileOpened(filename, m_imageStream->numFrames(), m_fps);
    if (m_tracker) {
        m_tracker->inputChanged();
        if (m_somethingIsLoaded && m_lastFilename.compare(filename) != 0) {
            m_tracker->onFileChanged(filename);
            m_lastFilename = filename;
        }
    }
}

void TrackingThread::run() {
    std::chrono::system_clock::time_point t;
    bool firstLoop = true;
    bool ignoreFps = false;

    while (true) {
        std::unique_lock<std::mutex> lk(m_tickMutex);


        m_conditionVariable.wait(lk, [&] {return (m_playing || m_playOnce) && !m_isRendering;});
        m_isRendering = true;

        ignoreFps = m_playOnce && !m_playing;

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
        double runningFps = 1000000. / std::chrono::duration_cast<std::chrono::microseconds>
                            (dur + target_dur).count();

        if (ignoreFps) {
            runningFps = -1;
        }

        tick(runningFps);

        std::this_thread::sleep_for(target_dur);
        t = std::chrono::system_clock::now();

        lk.unlock();
    }
}

void TrackingThread::tick(const double fps) {
    m_renderMutex.lock();
    std::string fileName = m_imageStream->currentFilename();

    if (!m_ignoreFilenameChanged) {
        // notify the tracker that the filename has changed.
        // This event will not occure when the camera
        // is used, otherwise it should be fired whenever a
        // new file is selected (video vs. set of images)
        if (m_lastFilename.compare(fileName) != 0) {
            if (m_tracker) {
                m_tracker->onFileChanged(fileName);
            }
            m_lastFilename = fileName;
        }
    }

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
        MutexLocker trackerLock(m_trackerMutex);
        if (m_tracker) {
            m_tracker->setCurrentFrameNumber(static_cast<int>(frameNumber));
        }
        playOnce();
    }
    m_renderMutex.unlock();
}
void TrackingThread::nextFrame() {
    if (m_imageStream->nextFrame()) { // increments the frame number if possible
        MutexLocker trackerLock(m_trackerMutex);
        if (m_tracker) {
            m_tracker->setCurrentFrameNumber(static_cast<int>(m_imageStream->currentFrameNumber()));
        }
    } else {
        m_playing = false;
        m_playOnce = false;
    }
}

void TrackingThread::doTracking() {
    MutexLocker trackerLock(m_trackerMutex);
    if (m_tracker) {
        // do nothing if we aint got a frame
        if (!m_imageStream->currentFrameIsEmpty()) {
            try {
                m_tracker->attemptTracking(m_imageStream->currentFrameNumber(),
                                           m_imageStream->currentFrame());
            } catch (const std::exception &err) {
                Q_EMIT notifyGUI("critical error in selected tracking algorithm: " +
                                 std::string(err.what()), MessageType::FAIL);
            }
        }
    }

    // we need to store the last tracked framenumber to use this value
    // in the paint methods. We MUST NOT use "m_imageStream->currentFrameNumber()"
    // in the paint methods as the value might be one off due to multithreading!
    // !!! (We cannot guarenty when "nextFrame" is called) !!!
    m_lastTrackedFrameNumber = m_imageStream->currentFrameNumber();
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
    return m_lastTrackedFrameNumber;
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
    m_texture.set(m_imageStream->currentFrame());
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
        QObject::connect(m_tracker.get(), &TrackingAlgorithm::pausePlayback,
                         this, &TrackingThread::requestPauseFromTracker);
        m_tracker.get()->postConnect();
        if (!m_ignoreFilenameChanged && m_somethingIsLoaded) {
            m_tracker->onFileChanged(m_imageStream->currentFilename());
            m_lastFilename = m_imageStream->currentFilename();
        }

        if (!m_isTrackingEnabled) {
            m_tracker->setTracking(false);
        }
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
    painter.drawRect(QRect(0, 0, static_cast<int>(w), static_cast<int>(h)));
    painter.setBrush(QColor(0, 0, 0, 0));

    m_paintMutex.lock();
    // using painters algorithm to draw in the right order
    if (m_somethingIsLoaded) {
        ProxyMat proxy(m_imageStream->currentFrame());

        {
            MutexLocker trackerLock(m_trackerMutex);
            if (m_tracker) {
                m_tracker.get()->paint(m_lastTrackedFrameNumber, proxy, v);
            }
        }

        if (proxy.isModified() || (m_lastFrameNumber != m_lastTrackedFrameNumber)) {
            m_texture.set(proxy.getMat());
            m_lastFrameNumber = m_lastTrackedFrameNumber;
        }

        // We use setWindow and setViewport to fit the video into the
        // given video widget frame (with width "w" and height "h")
        // we later need to adjust an offset caused the use of different
        // dimensions for window and viewport.

        // adjust the panning as the viewport is potentially scewed
        // and mouse movements given by the window are not translated
        // one-to-one anymore
        QRect window;
        QRect viewport;
        const float viewport_skew = ScreenHelper::calculate_viewport(
                                        m_texture.width(),
                                        m_texture.height(),
                                        static_cast<int>(w),
                                        static_cast<int>(h),
                                        window,
                                        viewport
                                    );

        painter.setWindow(window);
        painter.setViewport(viewport);

        float zoomFactor = 1 + zoom.zoomFactor;

        painter.scale(zoomFactor, zoomFactor);

        painter.translate(
            QPointF(
                -zoom.panX * viewport_skew / zoomFactor,
                -zoom.panY * viewport_skew / zoomFactor
            )
        );

        // TODO only paint that part that is visible
        painter.drawImage(
            QRectF(0, 0 , m_texture.width(), m_texture.height()),
            m_texture.get(),
            QRect(0, 0, m_texture.width(), m_texture.height()));

        {
            MutexLocker trackerLock(m_trackerMutex);
            if (m_tracker) {
                m_tracker.get()->paintOverlay(m_lastTrackedFrameNumber, &painter, v);
            }
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

void BioTracker::Core::TrackingThread::notifyGUIFromTracker(std::string m, MessageType type) {
    Q_EMIT notifyGUI(m, type);
}

void BioTracker::Core::TrackingThread::requestPauseFromTracker(bool pause) {
    if (pause) {
        setPause();
    } else {
        setPlay();
    }
}

void BioTracker::Core::TrackingThread::enableTracking() {
    m_isTrackingEnabled = true;
    if (m_tracker) {
        m_tracker->setTracking(true);
    }
}

void BioTracker::Core::TrackingThread::disableTracking() {
    m_isTrackingEnabled = false;
    if (m_tracker) {
        m_tracker->setTracking(false);
    }
}

}
}
