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
#include <QDir>
#include <QTemporaryDir>

//Open Tracker
#include "source/settings/Messages.h"
#include "source/helper/CvHelper.h"
#include "source/helper/StringHelper.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingThread.h"
#include "source/video/VideoView.h"
#include "source/tracking/TrackingAlgorithm.h"
#include "source/tracking/algorithm/simpletracker/SimpleTracker.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/BeesBookTagMatcher.h"
#include "source/tracking/algorithm/colorPatchTracker/colorPatchTracker.h"
#include "source/tracking/algorithm/fishparticle/ParticleFishTracker.h"
#include "source/tracking/algorithm/SampleTracker/SampleTracker.h"

#include "ui_BioTracker.h"

Q_DECLARE_METATYPE(cv::Mat)
	class TrackingThread;
	class VideoView;
	class TrackingAlgorithm;


class BioTracker: public QMainWindow
{
	Q_OBJECT

public:
	BioTracker(Settings &settings, QWidget *parent = 0,  Qt::WindowFlags flags = 0);
public slots:
	// open file browser for video in/out
	void browseVideo();
	// open file browser for picture in/out
	void browsePicture();
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
	//change video playback speed
	void changeFps(int fps);
	//different tracking algorithm was selected
	void trackingAlgChanged(QString trackingAlg);


	// SLOTS FOR TRACKING THREAD: 	
	void updateFrameNumber(int frameNumber);	
	void drawImage(cv::Mat image);
	void showFps(double fps);


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
	void changeCurrentFramebySlider();
	void changeCurrentFramebySlider(int SliderAction);
	void changeCurrentFramebyEdit();

	void takeScreenshot();
	


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
	void initCapture();
	void initAlgorithms();
	void initPicture(QStringList filenames);
	void connectTrackingAlg(TrackingAlgorithm* tracker);


	
signals:
	// tell tracking thread to grab next frame
	void nextFrameReady(bool);

	// tell tracking thread to grab specific frame
	void changeFrame(int);

	//tell Tracker to stop
	void videoStop();

	// tell tracking thread to pause/unpause video
	void videoPause(bool);	

	//tell tracking thread to grab next frame
	void grabNextFrame();

	//tell tracking thread to change playback speed
	void fpsChange(double fps);
	
	//enable max playback speed
	void enableMaxSpeed (bool enabled);

	//change tracking algorithm
	void changeTrackingAlg(TrackingAlgorithm &trackingAlgorithm);

};

#endif // BioTracker_H
