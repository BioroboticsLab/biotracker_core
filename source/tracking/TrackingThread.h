#ifndef TrackingThread_h
#define TrackingThread_h

#include <cv.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <QtCore/QThread>
//#include <video/Rectification.h>
#include <source/settings/Settings.h>
#include <source/settings/Messages.h>
#include <source/trackedObject/TrackedObject.h>
#include <source/tracking/TrackingAlgorithm.h>
#include <QWaitCondition>

class TrackingThread : public QThread
{
	Q_OBJECT
public:
	TrackingThread(Settings &settings);
	~TrackingThread(void);
	
	/**
	* Starts the video thread.
	* @return: void.
	*/
	void startCapture();

	/**
	* Loads in pictures instead of a video
	*/
	void loadPictures(QStringList filenames);

	/**
	* Reset
	* @return void.
	*/
	void resetTracker();


	/**
	* Checks if thread can handle next frame.
	* @return: true if it can, false otherwise.
	*/
	bool isReadyForNextFrame();

	/**
	* Gets the length of the video.
	* @return: the video length.
	*/
	int getVideoLength();

	/**	
	* @return  current fps setting
	*/
	double getFps();


private:	

	/** 
	* Video handling.
	*/
	// For reading the video file or video stream
	cv::VideoCapture _capture;	
	
	//defines whether to use pictures as source or a video
	bool _pictureMode;
	bool _captureActive;
	bool _trackerActive;
	bool _readyForNextFrame;
	bool _videoPause;	
	double _fps;
	double _runningFps;
	int _frameNumber;
	bool _maxSpeed;
	QWaitCondition _pauseCond;
	
	//contains filenames of all pictures that were selected by user
	QStringList _pictureFiles;

	cv::Mat getPicture(int index);

	Settings &_settings;

	TrackingAlgorithm* _tracker;
	std::vector<TrackedObject> _trackedObjects;

	/**
	* Gets current frame number.
	* @return: the current frame number.
	*/
	int getFrameNumber();

	/**
	 * Increments the current frame number by 1 frame.
	 * @return: void.
	 */
	void incrementFrameNumber();

	/**
	* sends frame and everything else that is needed to selected 
	* tracking algorithm and receives output frame
	*/
	void doTracking(cv::Mat frame);

	
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
		* Stops the video.
		* @return: void.
		*/
		void stopCapture();

		/**
		* Sets the current frame number.
		* @param: frameNumber, specifies the current frame number.
		*/
		void setFrameNumber(int frameNumber);

		/**
		* Moves one frame forward
		*/
		void nextFrame();

		/**
		* change framerate
		*/
		void setFps(double fps);
		
		/**
		* enable maximum playback speed
		*/
		void setMaxSpeed (bool enabled);

		void setTrackingAlgorithm(TrackingAlgorithm &trackingAlgorithm);
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
		//send current fps to gui
		void sendFps(double fps);

};  

#endif // !TrackingThread_h
