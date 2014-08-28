#ifndef TrackingAlgorithm_H
#define TrackingAlgorithm_H

#include <cv.h>
#include <source/trackedObject/TrackedObject.h>
#include <vector>
#include <source/video/Rectification.h>
#include <source/settings/Settings.h>
#include <QWidget>


class TrackingAlgorithm{
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

	virtual void initGUI(QWidget& paramWidget, QWidget& toolWidget);

	/**
	* Resets the tracker.
	* @return: void.
	*/
	virtual void reset() = 0;

protected:
	Settings &_settings;

};


#endif // !TrackingAlgorithm_H
