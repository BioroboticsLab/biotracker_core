#ifndef TrackingThread_h
#define TrackingThread_h

#include <cv.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <QtCore/QThread>
//#include <video/Rectification.h>
#include <settings/Settings.h>
#include <settings/Messages.h>
#include <trackedObject/TrackedObject.h>
#include <tracking/TrackingAlgorithm.h>



class TrackingThread : public QThread
{
	Q_OBJECT
public:
	TrackingThread(Settings &settings);
	~TrackingThread(void);
	void setTrackingAlgorithm();
	/**
	* Starts the video thread.
	* @return: void.
	*/
	void startCapture();

	/**
	* Reset
	* @return void.
	*/
	void resetTracker();


	/**
	* Stops the video.
	* @return: void.
	*/
	void stopCapture();

	/**
	* Gets the length of the video.
	* @return: the video length.
	*/
	int getVideoLength();


private:	

	/** 
	* Video handling.
	*/
	// For reading the video file or video stream
	cv::VideoCapture _capture;

	bool _captureActive;
	bool _trackerActive;
	bool _readyForNextFrame;
	bool _videoPause;

	int _fps;
	int _runningFps;
	int _frameNumber;


	Settings &_settings;

	TrackingAlgorithm* _tracker;

	/**
	* Gets current frame number.
	* @return: the current frame number.
	*/
	int getFrameNumber();

	/**
	* Increments current frame number by 1.
	* @return: void.
	*/
	void incrementFrameNumber();

	/**
	* Gets current frame number and
	* increments it by 1.
	* @return: the current frame number.
	*/
	int getAndIncrementFrameNumber();

	/**
	* Checks if thread can handle next frame.
	* @return: true if it can, false otherwise.
	*/
	bool isReadyForNextFrame();

	/**
	* Checks if thread is in pause state.
	* @return: true if paused, false otherwise.
	*/
	bool isVideoPause();

	/**
	* Checks if the tracker is on.
	* @return: true if tracker is on, false otherwise.
	*/ 
	bool isCaptureActive();

	/**
	* Set the capture in active or in-active state
	* @param: enabled, if true capture will be activated, false otherwise
	* @return: void.
	*/
	void enableCapture(bool);

	/**
	* Initializes the reading capture.
	* @return: void.
	*/
	void initCaptureForReadingVideoOrStream();


	/** 
	* thread running method.
	*/
	void run();

	public slots:
		void enableHandlingNextFrame(bool nextFrame);
		void enableVideoPause(bool videoPause);

		/**
		* Sets the current frame number.
		* @param: frameNumber, specifies the current frame number.
		*/
		void setFrameNumber(int frameNumber);


signals:
		/**
		* Signals when a tracking sequence is done.
		* @param: image, send the image to draw,
		* @return: void.
		*/
		void trackingSequenceDone(cv::Mat image);

		/**
		* emit current frame number.
		* @param: frameNumber, the current frame number.
		*/
		void newFrameNumber(int frameNumber);

		/**
		* Signal to run the capture thread.
		*/
		void enableCaptureThread();
		void disableCaptureThread();

		/**
		* send a message to the GUI.
		*/
		void notifyGUI(std::string message, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION);

};  

#endif // !TrackingThread_h
