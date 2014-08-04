#ifndef VideoHandler_h
#define VideoHandler_h

#include <QtCore/QMutex>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QDateTime>

#include <opencv2/opencv.hpp>

const double FpsAuto = -1.0;


class VideoHandler : public QThread
{
	Q_OBJECT

public:
	VideoHandler(cv::VideoCapture& capture, double fps = -1, std::string outputFile = "");

	/**
	* The standard destructor.
	* @return: void.
	*/
	virtual ~VideoHandler();

	/**
	* The running thread.
	* @return: void.
	*/
	void run();

	/**
	* Checks if the capture is opened to read frames.
	* @return: void.
	*/
	bool isOpened() { return _capture.isOpened(); }

	/**
	* This method is to read the frame, after is was retrieve from capture object..
	* @param: dest, the frame read from capture is copied into the the dest object.
	* @return: true, if frame could be read, false otherwise.
	*/
	bool readFrame(cv::Mat& dest);

	/**
	* Reads the current fps of the capture.
	* @return: the frame per second as double value.
	*/
	double fps();

	/**
	* Sets the FPS of the capture.
	* @param: fps, frame per second.
	*/
	void updateFPS(double fps);

	/**
	* Gets a customized output file
	* @param: filename, the file name.
	* @return: the output file name.
	*/
	static std::string getOutputFile(std::string filename = "");

	public slots:
		/**
		* Enable the stop requested flag.
		* @return: void.
		*/
		void stopRequested();

		/**
		* Disable the stop requested flag.
		* @return: void.
		*/
		void runningRequested();

protected:

	cv::VideoCapture _capture;
	cv::VideoWriter *_writer;

	int _delay_ms;
	double _fps;	
	bool _frameAvailable;
	bool _stopRequested;

	cv::Mat _frame;

	QMutex _frameMutex;
	QMutex _stopMutex;

private:
	/**
	* Checks if stop thread is requested.
	* @return: true, if stop is requeted, false otherwise.
	*/
	bool isStopRequested();

signals:
	void changeState(int);
};


class FileVideoHandler : public VideoHandler
{
public:
	FileVideoHandler(std::string inputFile, double fps = -1, std::string outputFile = "");
};


class CameraVideoHandler : public VideoHandler
{
public:
	CameraVideoHandler(double fps, std::string outputFile = "", int cam = 0);
};  
#endif // !VideoHandler_h
