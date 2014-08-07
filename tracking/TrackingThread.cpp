#include "TrackingThread.h"

#include <QFileInfo>
#include <time.h>

//#include "helper/CvHelper.h"
#include "settings/Param.h"

/**
* Mutexes.
*/

QMutex captureActiveMutex;
QMutex videoPauseMutex;
QMutex frameNumberMutex;
QMutex readyForNexFrameMutex;
QMutex trackerMutex;

TrackingThread::TrackingThread(Settings &settings) :	
	_settings(settings),
	_captureActive(false),
	_fps(30),
	_readyForNextFrame(true),
	_videoPause(false),
	_frameNumber(0),
	_tracker(NULL)
{
	_trackerActive =_settings.getValueOfParam<bool>(TRACKERPARAM::TRACKING_ENABLED);

	//setTrackingAlgorithm();	
}

TrackingThread::~TrackingThread(void)
{
	if(_tracker) delete _tracker;
}





void TrackingThread::startCapture()
{
	if(!isCaptureActive())
	{
		_capture = cv::VideoCapture(_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE));	
		enableCapture(true);
		QThread::start();
	}
}

void TrackingThread::stopCapture()
{		
	enableCapture(false);
	setFrameNumber(0);
}

void TrackingThread::run()
{
	cv::Mat frame;

	while(isCaptureActive())
	{	
		// when pause event is started.
		if(isVideoPause())
		{
			continue;
		}
		if(isReadyForNextFrame()){
			_capture.set(CV_CAP_PROP_POS_FRAMES, getAndIncrementFrameNumber());
			// capture the frame
			_capture >> frame;	

			// exit if last frame is reached
			//TODO: need to check memory violation thing here!
			if(frame.empty())	{	break;	}

			//TODO: if a tracking algorithm is selected
			//send frame to tracking algorithm
			


			// lock for handling the frame: for GUI, when GUI is ready, next frame can be handled.
			enableHandlingNextFrame(false);

			// lets GUI draw the frame.
			emit trackingSequenceDone(frame);
			emit newFrameNumber(getFrameNumber() - 1);
		}
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

void TrackingThread::incrementFrameNumber()
{
	int videoLength = _capture.get(CV_CAP_PROP_FRAME_COUNT);
	QMutexLocker locker(&frameNumberMutex);
	if(_frameNumber < videoLength)
		++_frameNumber;
}

void TrackingThread::setFrameNumber(int frameNumber)
{
	int videoLength = _capture.get(CV_CAP_PROP_FRAME_COUNT);
	QMutexLocker locker(&frameNumberMutex);
	if(frameNumber >= 0 && frameNumber <= videoLength)
		_frameNumber = frameNumber;
}


int TrackingThread::getAndIncrementFrameNumber()
{
	int frameNumber;
	QMutexLocker locker(&frameNumberMutex);
	frameNumber = _frameNumber++;
	return frameNumber;
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
}

bool TrackingThread::isVideoPause()
{
	QMutexLocker locker(&videoPauseMutex);
	return _videoPause;
}

int TrackingThread::getVideoLength()
{
	return _capture.get(CV_CAP_PROP_FRAME_COUNT);
}


void TrackingThread::resetTracker()
{
	_tracker->reset();
}

void TrackingThread::setTrackingAlgorithm(){
}

void TrackingThread::initCaptureForReadingVideoOrStream(){
}
