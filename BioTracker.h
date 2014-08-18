#ifndef BioTracker_H
#define BioTracker_H

#include <time.h>

// Open CV
#include <cv.h>

// QT 
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtCore/QString>

//Open Tracker
#include "settings/Messages.h"
#include "helper/CvHelper.h"
#include "helper/StringHelper.h"
#include "ui_BioTracker.h"
#include <settings/Settings.h>
#include <tracking/TrackingThread.h>
#include <video/VideoView.h>

Q_DECLARE_METATYPE(cv::Mat)
	class TrackingThread;
	class VideoView;


class BioTracker: public QMainWindow
{
	Q_OBJECT

public:
	BioTracker(Settings &settings, QWidget *parent = 0,  Qt::WindowFlags flags = 0);
public slots:
	// open file browser for video in/out
	void browseVideo();
	//checks current state (stopped,paused or playing)
	//and then sends appropriate signal to tracking thread
	void runCapture();
	// stops video thread
	void stopCapture();
	// next frame 
	void stepCaptureForward();
	// previous frame
	void stepCaptureBackward();
	//pause video thread;
	void pauseCapture();


	// signal emitted by tracking thread: update the current frameNumber
	void updateFrameNumber(int frameNumber);
	// signal emitted by tracking thread (a.k.a. "new tracking data available")
	void drawImage(cv::Mat image);


	/**
	 * Print proivided message to the GUI message area.
	 * @param: message, the message to print.
	 * @return: void.
	 */
	void printGuiMessage(std::string message, MSGS::MTYPE mType = MSGS::MTYPE::NOTIFICATION);

	/**
	 * Sets the algorithm used for tracking
	 * @param: trackingAlgId, the id of the algorithm.
	 */
	
	//void setTrackingAlg(int trackingAlgId);


	//void changeCurrentFrame();
	void changeCurrentFrameBySlider(int value);
	


private:
	Ui::BioTrackerClass ui;

/*	Rectification _rectification;
*/
	TrackingThread* _trackingThread;	

	Settings& _settings;
	bool _videoPaused;
	bool _videoStopped;
	int _currentFrame;
	QIcon _iconPause;
	QIcon _iconPlay;
	//disable or enable buttons for video navigating
	void setPlayfieldPaused(bool enabled);
	void init();
	void initGui();	
	void initConnects();


	
signals:
	// tell tracking thread to grab next frame
	void nextFrameReady(bool);



	// tell tracking thread to grab specific frame
	void changeFrame(int);


	// tell tracking thread to pause video
	void videoPause(bool);	

	//tell tracking thread to grab next frame
	void grabNextFrame();

};

#endif // BioTracker_H
