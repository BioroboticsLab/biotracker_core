#include "TrackingThread.h"

#include <QFileInfo>
#include <time.h>

#include "helper/CvHelper.h"
#include "settings/Messages.h"
#include "settings/Param.h"
#include "tracking/algorithm/simpletracker/SimpleTracker.h"
#include "helper/StringHelper.h"

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

	// TODO: remove, this ist just for testing
	//_tracker = new SimpleTracker(_settings);
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
		if (! _capture.isOpened())
		{
			// could not open video
			std::string errorMsg = "unable to open file " + _settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE);
			emit notifyGUI(errorMsg, MSGS::MTYPE::FAIL);
			return;
		}
		std::string note = "open file: " + _settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE);
		emit notifyGUI(note, MSGS::MTYPE::NOTIFICATION);
		enableCapture(true);
		_fps = _capture.get(CV_CAP_PROP_FPS);
		QThread::start();
	}
}

void TrackingThread::stopCapture()
{	
	//stop thread
	enableCapture(false);
	setFrameNumber(0);
	emit newFrameNumber(0);
}
// alternative way to calculate sleep time to get target fps described here:
// https://aaka.sh/patel/2013/06/28/live-video-webcam-recording-with-opencv/
void TrackingThread::run()
{
	cv::Mat frame;
	clock_t t;
	bool firstLoop = true;

	while(isCaptureActive())
	{	
		// when pause event is started.
		if(isVideoPause())
		{
			firstLoop = true;
			continue;
		}
		//if thread just started (or is unpaused) start clock here
		//after this timestamp will be taken right before picture is drawn 
		//to take the amount of time into account it takes to draw the picture
		if(firstLoop)
			t = clock();
		if(isReadyForNextFrame()){
			// measure the capture start time			
			if (!_capture.isOpened())	{	break;	}

			// capture the frame
			_capture >> frame;	
			incrementFrameNumber();

			// exit if last frame is reached
			//TODO: need to check memory violation thing here!
			if(frame.empty())	{	break;	}

			//TODO: if a tracking algorithm is selected
			//send frame to tracking algorithm
			 //NOTE: this is just for testing!
			if (_tracker) {
				frame = _tracker->track(std::vector<TrackedObject>(), getFrameNumber(), frame);
			}
			// lock for handling the frame: for GUI, when GUI is ready, next frame can be handled.
			enableHandlingNextFrame(false);
			t = clock() - t;
			double ms = 1000 / _fps;			
			if (t <= ms)
				ms -= ((double) t);
			else {	
				ms = 0;
			}

			// calculate the running fps.
			_runningFps = 1000 / (double)(t + ms);
			emit sendFps(_runningFps);
			msleep(ms);
			t = clock();
			firstLoop = false;

			// lets GUI draw the frame.
			emit trackingSequenceDone(frame.clone());
			emit newFrameNumber(getFrameNumber());
		}
	}
	if(!isCaptureActive())
		_capture.release();
	_frameNumber = 0;
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
	//do nothing if tracker is busy
	if(isReadyForNextFrame())
	{	
		int videoLength = _capture.get(CV_CAP_PROP_FRAME_COUNT);
		QMutexLocker locker(&frameNumberMutex);
		if(frameNumber >= 0 && frameNumber <= videoLength)
		{
			_frameNumber = frameNumber;
		
			if(isCaptureActive())
			{
				enableHandlingNextFrame(false);
				_capture.set(CV_CAP_PROP_POS_FRAMES,_frameNumber);
				cv::Mat frame;
				_capture >> frame;
				cv::waitKey(1);
				
			//TODO: if a tracking algorithm is selected
			//send frame to tracking algorithm
			 //NOTE: this is just for testing!
			if (_tracker) {
				frame = _tracker->track(std::vector<TrackedObject>(), getFrameNumber(), frame);
			}
				emit trackingSequenceDone(frame);
			}			
		}
	}
}

void TrackingThread::incrementFrameNumber()
{
	int videoLength = _capture.get(CV_CAP_PROP_FRAME_COUNT);
	QMutexLocker locker(&frameNumberMutex);
	if(_frameNumber < videoLength)
		++_frameNumber;
}

void TrackingThread::nextFrame()
{
	// capture the frame
	cv::Mat frame;
	_capture >> frame;	
	incrementFrameNumber();

	// only works if last frame not yet reached
	//TODO: need to check memory violation thing here!
	if(!frame.empty())	
	{	
		//TODO: if a tracking algorithm is selected
		//send frame to tracking algorithm
		// NOTE: this is just for testing!
		if (_tracker) {
			frame = _tracker->track(std::vector<TrackedObject>(), getFrameNumber(), frame);
		}
		// lock for handling the frame: for GUI, when GUI is ready, next frame can be handled.
		enableHandlingNextFrame(false);

		// lets GUI draw the frame.
		emit trackingSequenceDone(frame);
		emit newFrameNumber(getFrameNumber());
	}
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

double TrackingThread::getFps()
{
	return _fps;
}
void TrackingThread::setFps(double fps)
{
	_fps = fps;
}
void TrackingThread::setTrackingAlgorithm(){
}

void TrackingThread::initCaptureForReadingVideoOrStream(){
}
