#ifndef TrackingAlgorithm_H
#define TrackingAlgorithm_H

#include <cv.h>
#include <source/trackedObject/TrackedObject.h>
#include <vector>
#include <source/settings/Settings.h>
#include <qvector2d.h>
#include <source/settings/Messages.h>
#include <QMouseEvent>


class TrackingAlgorithm : public QObject
{
	Q_OBJECT

public:
	TrackingAlgorithm( Settings & settings );
	virtual	~TrackingAlgorithm();

	/**

	* This function tracks the provided object list within the provided frame.
	* @param: objectList
	* @param: frame number
	* @param: frame
	*/
	virtual void track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame )	= 0;
	virtual void paint		( cv::Mat& image ) = 0; 
	/**
	* Resets the tracker.
	* @return: void.
	*/
	virtual void reset() = 0;

public slots:
	//mouse click and move events
	void mouseMoveEvent		( QMouseEvent * e );
	void mousePressEvent	( QMouseEvent * e );
	void mouseReleaseEvent	( QMouseEvent * e );
	
signals:
	/**
	* send a message to the GUI.
	*/
	void notifyGUI			( std::string message, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION );


protected:
	Settings & _settings;

};


#endif // !TrackingAlgorithm_H
