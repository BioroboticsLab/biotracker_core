#ifndef TrackingAlgorithm_H
#define TrackingAlgorithm_H

#include <cv.h>
#include <vector>
#include <qvector2d.h>
#include <QMouseEvent>
#include <qwidget.h>
#include <fstream>
#include <memory>
#include <typeinfo>

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include <source/settings/Settings.h>
#include <source/settings/Messages.h>
#include "source/tracking/trackedObject/TrackedObject.h"

class TrackingAlgorithm : public QObject
{
	Q_OBJECT

public:
    TrackingAlgorithm( Settings& settings, std::string& serializationPath, QWidget *parent );
	virtual	~TrackingAlgorithm();

	/**

	* This function tracks the provided object list within the provided frame.
	* @param: objectList
	* @param: frame number
	* @param: frame
	*/
    virtual void track		( ulong frameNumber, cv::Mat& frame );

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
    virtual std::shared_ptr<QWidget> getToolsWidget();

	/**
	* getParamsWidget() will be called once at start up
	* to create a widget for gui with all 
	* parameter fields needed 
	*/
    virtual std::shared_ptr<QWidget> getParamsWidget();
	
	void loadObjects(std::vector<TrackedObject> &&objects);

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
    std::vector<TrackedObject> _trackedObjects;
    std::string _serializationPathName;
};

#endif // !TrackingAlgorithm_H
