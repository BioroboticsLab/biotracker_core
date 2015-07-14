#include "TrackingThread.h"

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
        _captureActive(false),
        _readyForNextFrame(true),
        m_status(TrackerStatus::NothingLoaded),
        _trackerActive(settings.getValueOfParam<bool>(TRACKERPARAM::TRACKING_ENABLED)),
        _fps(30),
        _runningFps(0),
        _maxSpeed(false),
        _mediaType(MediaType::NoMedia),
        _settings(settings),
        m_widget(nullptr),
        m_texture(&m_widget),
        _tracker(nullptr)
{}

TrackingThread::TrackingThread(Settings &settings, QOpenGLWidget *widget) :
    m_imageStream(make_ImageStreamNoMedia()),
    _captureActive(false),
    _readyForNextFrame(true),
    m_status(TrackerStatus::NothingLoaded),
    _trackerActive(settings.getValueOfParam<bool>(TRACKERPARAM::TRACKING_ENABLED)),
    _fps(30),
    _runningFps(0),
    _maxSpeed(false),
    _mediaType(MediaType::NoMedia),
    _settings(settings),
    m_widget(widget, nullptr),
    m_texture(&m_widget),
    _tracker(nullptr)
{}

TrackingThread::~TrackingThread(void)
{}

void TrackingThread::loadVideo(const boost::filesystem::path &filename)
{
	MutexLocker lock(_readyForNexFrameMutex);
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
        _fps = m_imageStream->fps();
		std::string note = "open file: " + _settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE) +
            " (#frames: " + QString::number(m_imageStream->numFrames()).toStdString() + ")";
		emit notifyGUI(note, MSGS::MTYPE::NOTIFICATION);
		emit fileNameChange(QString::fromStdString(_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE)));
		QThread::start();
	}
}

void TrackingThread::loadPictures(std::vector<boost::filesystem::path> &&filenames)
{
    MutexLocker lock(_readyForNexFrameMutex);
	_fps = 1;
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

		// load next frame
			{
                if ( !m_imageStream->nextFrame() )
                {
                    enableCapture(false);
                }
			}
		incrementFrameNumber();

		doTracking();
		std::chrono::microseconds target_dur(static_cast<int>(1000000. / _fps));
		std::chrono::microseconds dur =
			std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now() - t);
		if (!_maxSpeed)
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
		_runningFps = 1000000. / std::chrono::duration_cast<std::chrono::microseconds>(dur + target_dur).count();
		emit sendFps(_runningFps);
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
                m_texture.setImage(m_imageStream->currentFrame().clone());
                m_widget.makeCurrent();
                m_texture.draw();
                //emit trackingSequenceDone(m_imageStream->currentFrame().clone());
			}
			emit newFrameNumber(getFrameNumber());
		}
    }
}

void TrackingThread::enableCapture(bool enabled)
{
	MutexLocker lock(_captureActiveMutex);
	_captureActive = enabled;
}

bool TrackingThread::isCaptureActive()
{
	MutexLocker lock(_captureActiveMutex);
	return _captureActive;
}

void TrackingThread::setFrameNumber(size_t frameNumber)
{
	{
        if ( m_imageStream->setFrameNumber(frameNumber) )
		{
            MutexLocker lock(_trackerMutex);
            if (_tracker) {
                _tracker->setCurrentFrameNumber(frameNumber);
            }
		}
    }
    doTracking();
	{
        m_texture.setImage(m_imageStream->currentFrame());
        m_widget.makeCurrent();
        m_texture.draw();
        //emit trackingSequenceDone(m_imageStream->currentFrame());
        emit newFrameNumber(frameNumber);
	}
}

void TrackingThread::incrementFrameNumber()
{

        emit newFrameNumber(m_imageStream->currentFrameNumber());
        {
        MutexLocker lock(_trackerMutex);
        if (_tracker) {
            _tracker->setCurrentFrameNumber(m_imageStream->currentFrameNumber());
        }
    }
}

void TrackingThread::nextFrame()
{	
    if( m_imageStream->nextFrame() )
    {
        doTracking();
        // lock for handling the frame: for GUI, when GUI is ready, next frame can be handled.
        enableHandlingNextFrame(false);

        // lets GUI draw the frame.
        m_texture.setImage(m_imageStream->currentFrame());
        m_widget.makeCurrent();
        m_texture.draw();

        //emit trackingSequenceDone(m_imageStream->currentFrame());
        incrementFrameNumber();
    }
}


void TrackingThread::doTracking()
{
	MutexLocker trackerLock(_trackerMutex);
	if (!_tracker) return;

	// do nothing if we aint got a frame
    if (m_imageStream->currentFrameIsEmpty()) return;
	try
	{
        _tracker->track(m_imageStream->currentFrameNumber(), m_imageStream->currentFrame());
	}
	catch (const std::exception& err)
	{
		emit notifyGUI("critical error in selected tracking algorithm: " + std::string(err.what()), MSGS::FAIL);
	}
}
void TrackingThread::doTrackingAndUpdateScreen()
{
	doTracking();
    m_texture.setImage(m_imageStream->currentFrame());
    m_widget.makeCurrent();
    m_texture.draw();
    //emit trackingSequenceDone(m_imageStream->currentFrame());
}

size_t TrackingThread::getVideoLength() const
{
    return m_imageStream->numFrames();
}

void TrackingThread::mouseEvent(QMouseEvent *event)
{
    MutexLocker lock(_trackerMutex);
    if (_tracker) {
        QCoreApplication::sendEvent(_tracker.get(), event);
    }
}

void TrackingThread::keyboardEvent(QKeyEvent *event)
{
    MutexLocker lock(_trackerMutex);
    if (_tracker) {
        QCoreApplication::sendEvent(_tracker.get(), event);
    }
}

size_t TrackingThread::getFrameNumber() const
{
    return m_imageStream->currentFrameNumber();
}

void TrackingThread::enableHandlingNextFrame(bool nextFrame)
{
	MutexLocker lock(_readyForNexFrameMutex);
	_readyForNextFrame = nextFrame;
}

bool TrackingThread::isReadyForNextFrame()
{
	MutexLocker lock(_readyForNexFrameMutex);
	return _readyForNextFrame;
}

void TrackingThread::enableVideoPause(bool videoPause)
{
	_videoPause = videoPause;
    if(videoPause){
        m_status = TrackerStatus::Running;
    } else {
        m_status = TrackerStatus::Paused;
    }
}

bool TrackingThread::isVideoPause() const
{
	return _videoPause;
}


double TrackingThread::getFps() const
{
	return _fps;
}

void TrackingThread::stop()
{
	{
		MutexLocker lock(_captureActiveMutex);
		_captureActive = false;
	}
	QThread::wait();
	return;
}

void TrackingThread::setFps(double fps)
{
	_fps = fps;
}
void TrackingThread::setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm>  trackingAlgorithm)
{
	{
		MutexLocker lock(_trackerMutex);
		_tracker = trackingAlgorithm;
	}
	doTrackingAndUpdateScreen();
}

void TrackingThread::initCaptureForReadingVideoOrStream()
{
}
void TrackingThread::setMaxSpeed(bool enabled)
{
	_maxSpeed = enabled;
}

}
}
