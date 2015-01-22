#include "TrackingThread.h"

#include <QFileInfo>
#include <chrono>
#include <thread>

#include "source/settings/Messages.h"
#include "source/settings/Settings.h"
#include "source/settings/ParamNames.h"

using GUIPARAM::MediaType;

TrackingThread::TrackingThread(Settings &settings) :
_captureActive(false),
_readyForNextFrame(true),
_frameNumber(0),
_videoPause(false),
_trackerActive(settings.getValueOfParam<bool>(TRACKERPARAM::TRACKING_ENABLED)),
_fps(30),
_runningFps(0),
_maxSpeed(false),
_mediaType(MediaType::NoMedia),
_settings(settings),
_tracker(nullptr)
{}

TrackingThread::~TrackingThread(void)
{}

void TrackingThread::loadVideo(const std::string &filename)
{
	MutexLocker lock(_readyForNexFrameMutex);
	if (!isCaptureActive())
	{
		_capture = cv::VideoCapture(filename);
		if (!_capture.isOpened())
		{
			// could not open video
			std::string errorMsg = "unable to open file " + filename;
			emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
			emit invalidFile();
			return;
		}
		enableCapture(true);
		_mediaType = MediaType::Video;
		_fps = _capture.get(CV_CAP_PROP_FPS);
		std::string note = "open file: " + _settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE) +
			" (#frames: " + QString::number(getVideoLength()).toStdString() + ")";
		emit notifyGUI(note, MSGS::MTYPE::NOTIFICATION);
		emit fileNameChange(QString::fromStdString(_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE)));
		QThread::start();
	}
}

void TrackingThread::loadPictures(const std::vector<std::string> &&filenames)
{
	_mediaType = MediaType::Images;
	_pictureFiles = std::move(filenames);
	_fps = 1;
	enableCapture(true);
	QThread::start();
}

void TrackingThread::stopCapture()
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
			t = std::chrono::system_clock::now();

		// measure the capture start time
		if ((_mediaType == MediaType::Video) && !_capture.isOpened()) { break; }

		// load next frame
			{
				MutexLocker lock(_frameNumberMutex);
				switch (_mediaType) {
				case MediaType::Images:
					_frame = getPicture(_frameNumber);
					break;
				case MediaType::Video:
					_capture >> _frame;
                    break;
                 case MediaType::NoMedia:
                    break;
				default:
					assert(false);
					break;
				}
			}
		incrementFrameNumber();

		// exit if last frame is reached
		if (_frame.empty())	{ break; }

		//TODO: if a tracking algorithm is selected
		//send frame to tracking algorithm
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
				emit trackingSequenceDone(_frame.clone());
			}
			emit newFrameNumber(getFrameNumber());
		}


	}
	{
		MutexLocker lock(_frameNumberMutex);
		_frameNumber = 0;
	}
}

cv::Mat TrackingThread::getPicture(size_t index)
{
	//check if index in range
	if (index < _pictureFiles.size())
	{
		std::string filename = _pictureFiles.at(index);
		emit fileNameChange(QString::fromStdString(filename));
		return cv::imread(filename);
	}
	else
	{
		//return empty Picture if index out of range
		cv::Mat emptyPic;
		//QString filename = "-";
		//emit fileNameChange(filename);
		return emptyPic;
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

void TrackingThread::setFrameNumber(int frameNumber)
{
	const int videoLength = getVideoLength();

	{
		MutexLocker frameLocker(_frameNumberMutex);
		if (frameNumber >= 0 && frameNumber <= videoLength)
		{
			_frameNumber = frameNumber;

			switch (_mediaType) {
			case MediaType::Images:
				_frame = getPicture(_frameNumber);
				break;
			case MediaType::Video:
				_capture.set(CV_CAP_PROP_POS_FRAMES, _frameNumber);
				_capture >> _frame;
                break;
            case MediaType::NoMedia:
                break;
			default:
				assert(false);
				break;
			}

			{
				MutexLocker lock(_trackerMutex);
				if (_tracker) {
					_tracker->setCurrentFrameNumber(_frameNumber);
				}
			}
		}
	}

	doTracking();

	{
		MutexLocker frameLocker(_frameNumberMutex);
		emit trackingSequenceDone(_frame);
		emit newFrameNumber(_frameNumber);
	}
}

void TrackingThread::incrementFrameNumber()
{
	const int videoLength = getVideoLength();

	MutexLocker lock(_frameNumberMutex);

	if (_frameNumber < videoLength - 1)
	{
		++_frameNumber;

		emit newFrameNumber(_frameNumber);
	}
}

void TrackingThread::nextFrame()
{
	{
		MutexLocker lock(_frameNumberMutex);
		switch (_mediaType) {
		case MediaType::Images:
			_frame = getPicture(_frameNumber + 1);
			break;
		case MediaType::Video:
			_capture >> _frame;
            break;
        case MediaType::NoMedia:
            break;
		default:
			assert(false);
			break;
		}

	}
	incrementFrameNumber();

	// only works if last frame not yet reached
	//TODO: need to check memory violation thing here!
	if (!_frame.empty())
	{
		//TODO: if a tracking algorithm is selected
		//send frame to tracking algorithm
		// NOTE: this is just for testing!
		doTracking();

		// lock for handling the frame: for GUI, when GUI is ready, next frame can be handled.
		enableHandlingNextFrame(false);

		// lets GUI draw the frame.
		emit trackingSequenceDone(_frame);
		emit newFrameNumber(getFrameNumber());
	}
}


void TrackingThread::doTracking()
{
	MutexLocker trackerLock(_trackerMutex);
	if (!_tracker) return;

	// do nothing if we aint got a frame
	if (_frame.empty()) return;
	MutexLocker frameNumberLock(_frameNumberMutex);
	try
	{
		_tracker->track(_frameNumber, _frame);
	}
	catch (const std::exception& err)
	{
		emit notifyGUI("critical error in selected tracking algorithm: " + std::string(err.what()), MSGS::FAIL);
	}
}
void TrackingThread::doTrackingAndUpdateScreen()
{
	doTracking();
	emit trackingSequenceDone(_frame);
}

int TrackingThread::getFrameNumber()
{
	MutexLocker lock(_frameNumberMutex);
	return _frameNumber;
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
}

bool TrackingThread::isVideoPause() const
{
	return _videoPause;
}

int TrackingThread::getVideoLength()
{
	switch (_mediaType) {
	case MediaType::Images:
		return static_cast<int>(_pictureFiles.size());
	case MediaType::Video:
        return static_cast<int>(_capture.get(CV_CAP_PROP_FRAME_COUNT));
    case MediaType::NoMedia:
        return -1;
	default:
		assert(false);
		return -1;
	}
}

void TrackingThread::resetTracker()
{
	MutexLocker lock(_trackerMutex);
	_tracker->reset();
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
