#include "VideoHandler.h"

#include "settings/ParamNames.h"

VideoHandler::VideoHandler(cv::VideoCapture& capture, double fps, std::string outputFile) :
	_capture(capture),
	_writer(NULL),
	_frameAvailable(false),
	_stopRequested(false)
{
	if (fps == FpsAuto)
		_fps = capture.get(CV_CAP_PROP_FPS);
	else
		_fps = fps;

	_delay_ms = 1000/_fps;

	if (!outputFile.empty())
	{
		_writer = new cv::VideoWriter(outputFile, CV_FOURCC('D','I','V','X'), _fps,
			cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)));
	}
}

VideoHandler::~VideoHandler()
{
	if (_writer)
		delete _writer;
}

FileVideoHandler::FileVideoHandler(std::string inputFile, double fps, std::string outputFile) :
	VideoHandler(cv::VideoCapture(inputFile), fps, outputFile)
{}

CameraVideoHandler::CameraVideoHandler(double fps, std::string outputFile, int cam) :
	VideoHandler(cv::VideoCapture(cam), fps, outputFile)
{}

bool VideoHandler::isStopRequested()
{
	QMutexLocker locker(&_stopMutex);
	return _stopRequested;
}

void VideoHandler::stopRequested()
{
	QMutexLocker locker(&_stopMutex);
	_stopRequested = true;
}

void VideoHandler::runningRequested()
{
	QMutexLocker locker(&_stopMutex);
	_stopRequested = false;
}

void VideoHandler::run()
{
	// Thread started...
	while (!isStopRequested())
	{
		if (!_capture.isOpened())
		{
			std::cerr << "VideoHandler::run(): VideoCapture is not opened! Terminating VideoHandler";
			emit changeState((int)CAPTUREPARAM::CAP_STATE_ENUM::CAP_EXITED);			
			break;
		}

		_frameMutex.lock();
		_capture >> _frame;
		_frameMutex.unlock();

		if(_frame.empty())
		{
			emit changeState((int)CAPTUREPARAM::CAP_STATE_ENUM::CAP_FINISHED);
			break;
		}
			
		_frameAvailable = true;

		//if (_writer)
		//	*_wri*er << _frame;

		msleep(_delay_ms);
	}
}

bool VideoHandler::readFrame(cv::Mat& dest)
{
	if (_frameAvailable)
	{
		QMutexLocker locker(&_frameMutex);
		_frameAvailable = false;
		_frame.copyTo(dest);
		return true;
	}
	return false;
}

double VideoHandler::fps()
{
	return _capture.get(CV_CAP_PROP_FPS);
}

void VideoHandler::updateFPS(double fps)
{
	try
	{		
		_capture.set(CV_CAP_PROP_FPS,fps);
		_fps = fps;
		_delay_ms = 1000/_fps;		
	} 
	catch (cv::Exception)
	{
	}
}

std::string VideoHandler::getOutputFile(std::string filename)
{
	std::string outputFilename;

	QString dateTimeQString = QDateTime::currentDateTime().toString("DyyMMddThhmmss");
	std::string dateTimeStdString = dateTimeQString.toStdString();

	//TODO: need to check the file extension, split it and then append the output posfix.
	if(filename.empty()) 
	{
		outputFilename = std::string("output_file_from_cam_").append(dateTimeStdString);
	} else 
	{
		outputFilename = filename.append("_output_").append(dateTimeStdString);
	}

	return outputFilename;
}