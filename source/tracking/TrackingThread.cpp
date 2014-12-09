#include "TrackingThread.h"

#include <QFileInfo>
#include <QMutex>
#include <chrono>
#include <thread>

#include "source/settings/Messages.h"
#include "source/settings/Settings.h"
#include "source/settings/ParamNames.h"

/**
* Mutexes.
*/
QMutex captureActiveMutex;
QMutex videoPauseMutex;
QMutex frameNumberMutex;
QMutex readyForNexFrameMutex;
QMutex trackerMutex;

TrackingThread::TrackingThread(Settings &settings) :	
    _captureActive(false),
	_readyForNextFrame(true),
	_videoPause(false),
    _fps(30),
    _frameNumber(0),
    _maxSpeed(false),
    _settings(settings),
    _tracker(nullptr)
{
    _trackerActive =_settings.getValueOfParam<bool>(TRACKERPARAM::TRACKING_ENABLED);
}

TrackingThread::~TrackingThread(void)
{
}


void TrackingThread::startCapture()
{
	if(!isCaptureActive())
	{
        _capture = cv::VideoCapture(_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE));
		if (! _capture.isOpened())
		{
			// could not open video
			std::string errorMsg = "unable to open file " + _settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE);
			emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
			emit invalidFile();
			return;
		}
		enableCapture(true);
		_pictureMode = false;
		_fps = _capture.get(CV_CAP_PROP_FPS);
		QThread::start();
		std::string note = "open file: " + _settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE) + 
			" (#frames: " + QString::number(getVideoLength()).toStdString() + ")";
		emit notifyGUI(note, MSGS::MTYPE::NOTIFICATION);
	}
}

void TrackingThread::loadPictures(QStringList  filenames)
{
	_pictureMode = true;
	_pictureFiles = filenames;
	enableCapture(true);
	_fps = 1;
	QThread::start();	
}

void TrackingThread::stopCapture()
{	
	enableHandlingNextFrame(false);
	enableCapture(false);
	if(isVideoPause())
	{
		_pauseCond.wakeAll();		
	}
	if(!this->wait(3000)) //Wait until thread actually has terminated (max. 3 sec)
	{
		emit notifyGUI("Thread deadlock detected! Terminating now!",MSGS::FAIL);
		terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
		wait(); //Note: We have to wait again here!
	}
}

void TrackingThread::run()
{	
    std::chrono::system_clock::time_point t;
	bool firstLoop = true;

	while(isCaptureActive())
	{			
		// when pause event is started.
        while(isVideoPause())
		{
			QMutexLocker locker(&videoPauseMutex);
			firstLoop = true;
            _pauseCond.wait(&videoPauseMutex, 100);
            if (!isCaptureActive())
                return;
		}

		//if thread just started (or is unpaused) start clock here
		//after this timestamp will be taken right before picture is drawn 
		//to take the amount of time into account it takes to draw the picture
		if(firstLoop)
            t = std::chrono::system_clock::now();
		if(isReadyForNextFrame()){
			// measure the capture start time			
			if (!_capture.isOpened() && !_pictureMode)	{	break;	}

			if(_pictureMode)
			{
				incrementFrameNumber();
				_frame = getPicture(_frameNumber);
			}
			else
			{
				// capture the frame
				_capture >> _frame;
				incrementFrameNumber();
			}			

			// exit if last frame is reached
			if (_frame.empty())	{ break; }

			//TODO: if a tracking algorithm is selected
			//send frame to tracking algorithm			
			if (_tracker) {
				doTracking();
			}
			// lock for handling the frame: for GUI, when GUI is ready, next frame can be handled.
			enableHandlingNextFrame(false);

			std::chrono::microseconds target_dur(static_cast<int>(1000000. / _fps));
            std::chrono::microseconds dur =
                    std::chrono::duration_cast<std::chrono::microseconds>(
                        std::chrono::system_clock::now() - t);
			if(!_maxSpeed)
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

			if(isCaptureActive())
			{
				// lets GUI draw the frame.
				emit trackingSequenceDone(_frame.clone());
				emit newFrameNumber(getFrameNumber());
			}

		}
	}
	//if(!isCaptureActive())
	//	_capture.release();
	_frameNumber = 0;
}

cv::Mat TrackingThread::getPicture(int index)
{
	//check if index in range
	if(index < _pictureFiles.length() && index >= 0)
	{
		std::string filename = _pictureFiles.at(index).toStdString();
		return cv::imread(filename);
	}
	else
	{
		//return empty Picture if index out of range
		cv::Mat emptyPic;
		return emptyPic;
	}

}

void TrackingThread::enableCapture(bool enabled)
{	
	QMutexLocker locker(&captureActiveMutex);
	_captureActive = enabled;	
}

bool TrackingThread::isCaptureActive()
{
	QMutexLocker locker(&captureActiveMutex);		
	return _captureActive;
}


void TrackingThread::setFrameNumber(int frameNumber)
{
	const int videoLength = getFrameNumber();
	QMutexLocker frameLocker(&frameNumberMutex);

	if(frameNumber >= 0 && frameNumber <= videoLength)
	{
		_frameNumber = frameNumber;		
		if (_tracker) _tracker->setCurrentFrameNumber(frameNumber);
		_capture.set(CV_CAP_PROP_POS_FRAMES,_frameNumber);
		if(_pictureMode)
		{
			_frame = getPicture(_frameNumber);
		}
		else
		{
			_capture >> _frame;
		}

		if (_tracker) {
			doTracking();
		}
		emit trackingSequenceDone(_frame);
		emit newFrameNumber( _frameNumber );
	}	
}

void TrackingThread::incrementFrameNumber()
{
	const int videoLength = getVideoLength();
	QMutexLocker locker(&frameNumberMutex);
	if(_frameNumber < videoLength) {
		++_frameNumber;
		emit newFrameNumber(_frameNumber);
	}
}

void TrackingThread::nextFrame()
{
	// capture the frame
	if(_pictureMode)
	{
		_frame = getPicture(_frameNumber + 1);
	}
	else
	{
		_capture >> _frame;
	}
	incrementFrameNumber();

	// only works if last frame not yet reached
	//TODO: need to check memory violation thing here!
	if (!_frame.empty())
	{	
		//TODO: if a tracking algorithm is selected
		//send frame to tracking algorithm
		// NOTE: this is just for testing!
		if (_tracker) {
			doTracking();
		}
		// lock for handling the frame: for GUI, when GUI is ready, next frame can be handled.
		enableHandlingNextFrame(false);

		// lets GUI draw the frame.
		emit trackingSequenceDone(_frame);
		emit newFrameNumber(getFrameNumber());
	}
}


void TrackingThread::doTracking()
{
	//do nothing if we aint got a frame
	if (_frame.empty())
		return;
	QMutexLocker locker(&trackerMutex);
    try
    {
		_tracker->track( _frameNumber, _frame);
    }
	catch(const std::exception& err)
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
	QMutexLocker locker(&frameNumberMutex);
	return _frameNumber;
}

void TrackingThread::enableHandlingNextFrame(bool nextFrame)
{
	QMutexLocker locker(&readyForNexFrameMutex);
	_readyForNextFrame = nextFrame;
}

bool TrackingThread::isReadyForNextFrame()
{
	QMutexLocker locker(&readyForNexFrameMutex);
	return _readyForNextFrame;
}

void TrackingThread::enableVideoPause(bool videoPause)
{
	QMutexLocker locker(&videoPauseMutex);
	_videoPause = videoPause;
	if(!videoPause)
	{
		_pauseCond.wakeAll();
	}
}

bool TrackingThread::isVideoPause()
{
	QMutexLocker locker(&videoPauseMutex);
	return _videoPause;
}

int TrackingThread::getVideoLength()
{
	if(_pictureMode)
	{
		return _pictureFiles.length();
	}
	else
	{
		return _capture.get(CV_CAP_PROP_FRAME_COUNT);
	}
}


void TrackingThread::resetTracker()
{
	_tracker->reset();
}

double TrackingThread::getFps()
{
    return _fps;
}

void TrackingThread::stop()
{
    {
        QMutexLocker locker(&captureActiveMutex);
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
	QMutexLocker locker(&trackerMutex);
	_tracker = trackingAlgorithm;		
}

void TrackingThread::initCaptureForReadingVideoOrStream()
{
}
void TrackingThread::setMaxSpeed(bool enabled)
{
	_maxSpeed = enabled;
}
