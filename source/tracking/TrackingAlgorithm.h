#ifndef TrackingAlgorithm_H
#define TrackingAlgorithm_H

#include <cv.h>
#include <source/trackedObject/TrackedObject.h>
#include <vector>
#include <source/video/Rectification.h>
#include <source/settings/Settings.h>
#include <qvector2d.h>
#include <source/settings/Messages.h>


class TrackingAlgorithm : public QObject{
	Q_OBJECT
public:
	TrackingAlgorithm(Settings& settings);
	virtual ~TrackingAlgorithm();

	/**

	* This function tracks the provided object list within the provided frame.
	* @param: fishList, the reference list of the tracking fishes.
	* @param: frame, the reference image, which shall contain fishes.
	* @return: cv picture whis is to be drawn in video widget
	*/
	virtual cv::Mat track(std::vector<TrackedObject>& objectList, unsigned long frameNumber, cv::Mat frame) = 0;

	/**
	* Resets the tracker.
	* @return: void.
	*/
	virtual void reset() = 0;

public slots:
	//mouse click events for each button
	void mousePressLeft(QVector2D point);
	void mouseReleaseLeft(QVector2D point);
	void mousePressRight(QVector2D point);
	void mouseReleaseRight(QVector2D point);
	void mousePressMiddle(QVector2D point);
	void mouseReleaseMiddle(QVector2D point);
	
signals:
	/**
	* send a message to the GUI.
	*/
	void notifyGUI(std::string message, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION);


protected:
	Settings &_settings;

};


#endif // !TrackingAlgorithm_H
