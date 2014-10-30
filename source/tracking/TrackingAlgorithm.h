#ifndef TrackingAlgorithm_H
#define TrackingAlgorithm_H

#include <cv.h>
#include <source/tracking/trackedObject/TrackedObject.h>
#include <vector>
#include <source/settings/Settings.h>
#include <qvector2d.h>
#include <source/settings/Messages.h>
#include <QMouseEvent>
#include <qwidget.h>

#include <typeinfo>

class TrackingAlgorithm : public QObject
{
	Q_OBJECT

public:
	TrackingAlgorithm( Settings & settings, QWidget *parent );
	virtual	~TrackingAlgorithm();

	/**

	* This function tracks the provided object list within the provided frame.
	* @param: objectList
	* @param: frame number
	* @param: frame
	*/
	virtual void track		( ulong frameNumber, cv::Mat& frame )	= 0;

	/**
	* paint will be called by "VideoViews" paintGL method
	* so any picture manipulation stuff goes in here 
	*/
	virtual void paint		(cv::Mat& image ) = 0;

	/**
	* Resets the tracker. never called - yet
	* @return: void.
	*/
	virtual void reset() = 0;

	/**
	* getToolsFrame() will be called once at start up
	* to create a widget for gui with all 
	* buttons needed for interaction 
	*/
	virtual QWidget* getToolsWidget();

	/**
	* getParamsWidget() will be called once at start up
	* to create a widget for gui with all 
	* parameter fields needed 
	*/
	virtual QWidget* getParamsWidget();
	
	void loadObjects( std::vector<TrackedObject> * objects );

public slots:
	//mouse click and move events
	virtual void mouseMoveEvent		( QMouseEvent * e );
	virtual void mousePressEvent	( QMouseEvent * e );
	virtual void mouseReleaseEvent	( QMouseEvent * e );
	virtual void mouseWheelEvent	( QWheelEvent * e );
	
signals:
	/**
	* send a message to the GUI.
	*/
	void notifyGUI			( std::string message, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION );
	
	/**
	* send signal to VideoView and update display
	*/
	void update();

	cv::Mat requestCurrentScreen();


protected:
	Settings & _settings;
	QWidget * _parent;
	std::vector<TrackedObject> * _trackedObjects;

};


#endif // !TrackingAlgorithm_H
