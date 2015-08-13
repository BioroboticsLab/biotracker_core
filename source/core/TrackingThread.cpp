#include "TrackingThread.h"

#include <iostream>

#include <chrono>
#include <thread>

#include <QFileInfo>

#include "source/core/settings/Messages.h"
#include "source/core/settings/Settings.h"
#include "source/core/settings/ParamNames.h"

#include <QCoreApplication>
#include <QtOpenGL/qgl.h>

namespace BioTracker {
namespace Core {

using GUIPARAM::MediaType;

TrackingThread::TrackingThread(Settings &settings) :
    m_imageStream(make_ImageStreamNoMedia()),
    m_captureActive(false),
    m_readyForNextFrame(true),
    m_status(TrackerStatus::NothingLoaded),
    m_trackerActive(settings.getValueOfParam<bool>(TRACKERPARAM::TRACKING_ENABLED)),
    m_fps(30),
    m_runningFps(0),
    m_maxSpeed(false),
    m_mediaType(MediaType::NoMedia),
    m_settings(settings),
    m_texture(nullptr),
    m_openGLLogger(this)
{}

TrackingThread::~TrackingThread(void)
{}

void TrackingThread::initializeOpenGL(std::unique_ptr<Util::SharedOpenGLContext> &&context, TextureObject &texture)
{
    m_context = std::move(context);
    m_texture = &texture;
    std::cout << m_context->format().minorVersion() << std::endl;
    m_surface.setFormat(m_context->format());
    m_surface.create();

    m_openGLLogger.initialize(); // initializes in the current context, i.e. ctx
    connect(&m_openGLLogger, &QOpenGLDebugLogger::messageLogged, this, &TrackingThread::handleLoggedMessage);
    m_openGLLogger.startLogging();
}

void TrackingThread::loadVideo(const boost::filesystem::path &filename)
{
    MutexLocker lock(m_readyForNexFrameMutex);
	if (!isCaptureActive())
	{
        m_imageStream = make_ImageStreamVideo(filename);
        if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia)
		{
			// could not open video
            std::string errorMsg = "unable to open file " + filename.string();
			emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
			emit invalidFile();
            m_status = TrackerStatus::Invalid;
			return;
		}
        m_status = TrackerStatus::Paused;
        enableCapture(true);
        m_fps = m_imageStream->fps();

        // TODO: crashes at getValueOfParam
        std::string note = "abc";
//        std::string note = "open file: " + m_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE) +
//            " (#frames: " + QString::number(m_imageStream->numFrames()).toStdString() + ")";
        emit notifyGUI(note, MSGS::MTYPE::NOTIFICATION);
        //emit fileNameChange(QString::fromStdString(m_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE)));
        emit fileNameChange(QString::fromStdString(note));
        QThread::start();
	}
}

void TrackingThread::loadPictures(std::vector<boost::filesystem::path> &&filenames)
{
    MutexLocker lock(m_readyForNexFrameMutex);
    m_fps = 1;
    m_imageStream = make_ImageStreamPictures(std::move(filenames));
    if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia)
    {
        // could not open video
        std::string errorMsg = "unable to open files [";
        for(boost::filesystem::path filename: filenames){
            errorMsg += ", " + filename.string();
        }
        errorMsg += "]";
        emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
        emit invalidFile();
        m_status = TrackerStatus::Invalid;
        return;
    }
    m_status = TrackerStatus::Paused;
	enableCapture(true);
	QThread::start();
}

void TrackingThread::openCamera(int device)
{
    MutexLocker lock(m_readyForNexFrameMutex);
	if (!isCaptureActive())
	{
        m_imageStream = make_ImageStreamCamera(device);
        if (m_imageStream->type() == GUIPARAM::MediaType::NoMedia)
		{
			// could not open video
            std::string errorMsg = "unable to open camera " + QString::number(device).toStdString();
			emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
			emit invalidFile();
            m_status = TrackerStatus::Invalid;
			return;
		}
        m_status = TrackerStatus::Running;
        enableCapture(true);
        m_fps = m_imageStream->fps();
		std::string note = "open camera " + QString::number(device).toStdString();
		emit notifyGUI(note, MSGS::MTYPE::NOTIFICATION);
		QThread::start();
	}
}

void TrackingThread::terminateThread()
{
	enableHandlingNextFrame(false);
	enableCapture(false);
	if (!this->wait(3000)) //Wait until thread actually has terminated (max. 3 sec)
	{
		emit notifyGUI("Thread deadlock detected! Terminating now!", MSGS::FAIL);
		terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
		wait(); //Note: We have to wait again here!
	}
}

void TrackingThread::run()
{
    m_context->makeCurrent(&m_surface);
    std::cout << QOpenGLContext::currentContext() << std::endl;
    m_texture->setImage(m_imageStream->currentFrame().clone());
    m_context->doneCurrent();
    return;

	std::chrono::system_clock::time_point t;
	bool firstLoop = true;

	while (isCaptureActive())
	{
		// when pause event is started.
		while (isVideoPause())
		{
			firstLoop = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (!isCaptureActive()) return;
		}

		//if thread just started (or is unpaused) start clock here
		//after this timestamp will be taken right before picture is drawn
		//to take the amount of time into account it takes to draw the picture
		if (firstLoop)
            // measure the capture start time
			t = std::chrono::system_clock::now();


        if ((m_imageStream->type() == GUIPARAM::MediaType::Video) && m_imageStream->lastFrame() ) { break; }



		incrementFrameNumber();

		doTracking();
        std::chrono::microseconds target_dur(static_cast<int>(1000000. / m_fps));
		std::chrono::microseconds dur =
			std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now() - t);
        if (!m_maxSpeed)
		{
			if (dur <= target_dur)
				target_dur -= dur;
			else {
				target_dur = std::chrono::microseconds(0);
			}
		}
		else
			target_dur = std::chrono::microseconds(0);
		// calculate the running fps.
        m_runningFps = 1000000. / std::chrono::duration_cast<std::chrono::microseconds>(dur + target_dur).count();
        emit sendFps(m_runningFps);
		std::this_thread::sleep_for(target_dur);
		t = std::chrono::system_clock::now();
		firstLoop = false;

		if (isCaptureActive())
		{
			// lets GUI draw the frame.
			if (isReadyForNextFrame())
			{
				// lock for handling the frame: when GUI is ready, next frame can be handled.
                enableHandlingNextFrame(false);
                m_texture->setImage(m_imageStream->currentFrame().clone());

                //emit trackingSequenceDone(m_imageStream->currentFrame().clone());
			}
			emit newFrameNumber(getFrameNumber());
		}
        if ( !m_imageStream->nextFrame() )
        {
            enableCapture(false);
        }
    }
}

void TrackingThread::enableCapture(bool enabled)
{
    MutexLocker lock(m_captureActiveMutex);
    m_captureActive = enabled;
}

bool TrackingThread::isCaptureActive()
{
    MutexLocker lock(m_captureActiveMutex);
    return m_captureActive;
}

void TrackingThread::setFrameNumber(size_t frameNumber)
{
	{
        if ( m_imageStream->setFrameNumber(frameNumber) )
		{
            MutexLocker lock(m_trackerMutex);
            if (m_tracker) {
                m_tracker->setCurrentFrameNumber(frameNumber);
            }
		}
    }
    doTracking();
	{
        //m_videoView.setImage(m_imageStream->currentFrame().clone());
//        m_texture.setImage(m_imageStream->currentFrame());
//        m_widget.makeCurrent();
//        m_texture.draw();
        //emit trackingSequenceDone(m_imageStream->currentFrame());
        emit newFrameNumber(frameNumber);
	}
}

void TrackingThread::incrementFrameNumber()
{

        emit newFrameNumber(m_imageStream->currentFrameNumber());
        {
        MutexLocker lock(m_trackerMutex);
        if (m_tracker) {
            m_tracker->setCurrentFrameNumber(m_imageStream->currentFrameNumber());
        }
    }
}

void TrackingThread::nextFrame()
{
    if( m_imageStream->nextFrame() )
    {
        MutexLocker lock(m_contextNotCurrentMutex);
        m_texture->setImage(m_imageStream->currentFrame());
        doTracking();
        // lock for handling the frame: for GUI, when GUI is ready, next frame can be handled.
        enableHandlingNextFrame(false);

        // lets GUI draw the frame.
        //m_videoView.setImage(m_imageStream->currentFrame().clone());
//        m_texture.setImage(m_imageStream->currentFrame());
//        m_widget.makeCurrent();
//        m_texture.draw();

        //emit trackingSequenceDone(m_imageStream->currentFrame());
        incrementFrameNumber();
    }
}


void TrackingThread::doTracking()
{
    MutexLocker trackerLock(m_trackerMutex);
    if (!m_tracker) return;

	// do nothing if we aint got a frame
    if (m_imageStream->currentFrameIsEmpty()) return;
	try
	{
        m_tracker->track(m_imageStream->currentFrameNumber(), m_imageStream->currentFrame());
	}
	catch (const std::exception& err)
	{
		emit notifyGUI("critical error in selected tracking algorithm: " + std::string(err.what()), MSGS::FAIL);
	}
}
void TrackingThread::doTrackingAndUpdateScreen()
{
	doTracking();
    //m_videoView.setImage(m_imageStream->currentFrame().clone());
//    m_texture.setImage(m_imageStream->currentFrame());
//    m_widget.makeCurrent();
//    m_texture.draw();
    //emit trackingSequenceDone(m_imageStream->currentFrame());
}

size_t TrackingThread::getVideoLength() const
{
    return m_imageStream->numFrames();
}

void TrackingThread::mouseEvent(QMouseEvent *event)
{
    MutexLocker lock(m_trackerMutex);
    if (m_tracker) {
        QCoreApplication::sendEvent(m_tracker.get(), event);
    }
}

void TrackingThread::keyboardEvent(QKeyEvent *event)
{
    MutexLocker lock(m_trackerMutex);
    if (m_tracker) {
        QCoreApplication::sendEvent(m_tracker.get(), event);
    }
}

size_t TrackingThread::getFrameNumber() const
{
    return m_imageStream->currentFrameNumber();
}

void TrackingThread::enableHandlingNextFrame(bool nextFrame)
{
    MutexLocker lock(m_readyForNexFrameMutex);
    m_readyForNextFrame = nextFrame;
}

bool TrackingThread::isReadyForNextFrame()
{
    MutexLocker lock(m_readyForNexFrameMutex);
    return m_readyForNextFrame;
}

void TrackingThread::enableVideoPause(bool videoPause)
{
    m_videoPause = videoPause;
    if(videoPause){
        m_status = TrackerStatus::Running;
    } else {
        m_status = TrackerStatus::Paused;
    }
}

bool TrackingThread::isVideoPause() const
{
    return m_videoPause;
}


double TrackingThread::getFps() const
{
    return m_fps;
}

void TrackingThread::stop()
{
	{
        MutexLocker lock(m_captureActiveMutex);
        m_captureActive = false;
	}
	QThread::wait();
	return;
}

void TrackingThread::setFps(double fps)
{
    m_fps = fps;
}
void TrackingThread::setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm>  trackingAlgorithm)
{
	{
        MutexLocker lock(m_trackerMutex);
        m_tracker = trackingAlgorithm;
	}
	doTrackingAndUpdateScreen();
}

void TrackingThread::initCaptureForReadingVideoOrStream()
{
}
void TrackingThread::setMaxSpeed(bool enabled)
{
    m_maxSpeed = enabled;
}

void TrackingThread::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage) {
    std::cout << debugMessage.message().toStdString() << std::endl;
}

}
}
