#ifndef BioTracker_H
#define BioTracker_H

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
#include "ui_BioTracker.h"
//#include <Rectification.h>
//#include <Settings.h>
//#include <TrackingThread.h>



class BioTracker: public QMainWindow
{
	Q_OBJECT

public:
	BioTracker(QWidget *parent = 0,  Qt::WindowFlags flags = 0);
	~BioTracker();
public slots:


	/**
	 * Print proivided message to the GUI message area.
	 * @param: message, the message to print.
	 * @return: void.
	 */
//	void printGuiMessage(std::string message, MSGS::MTYPE mType = MSGS::MTYPE::NOTIFICATION);

	/**
	 * Sets the algorithm used for tracking
	 * @param: trackingAlgId, the id of the algorithm.
	 */
	/*
	void setTrackingAlg(int trackingAlgId);

	// signal emitted by tracking thread: update the current frameNumber
	void updateFrameNumber(int frameNumber);
	void changeCurrentFrame();
	void changeCurrentFrameBySlider(int value);
	*/


private:
	Ui::BioTrackerClass ui;

/*	Rectification _rectification;
	Settings _settings;
	TrackingThread _trackingThread;
*/
	bool _pauseVideo;
	int currentFrame;

	void initGui();
	/*
	void initConnects();

signals:
	// tell tracking thread to grab next frame
	void nextFrameReady(bool);

	// tell tracking thread to grab specific frame
	void changeFrame(int);

	// tell tracking thread to pause video
	void videoPause(bool);
	*/

};

#endif // BioTracker_H
