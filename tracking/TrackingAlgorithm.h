#ifndef TrackingAlgorithm_H
#define TrackingAlgorithm_H

#include <cv.h>
#include <trackedObject/TrackedObject.h>
#include <vector>
#include <video/Rectification.h>
#include <settings/Settings.h>


template <typename T>
class TrackingAlgorithm{
public:


	/**

	* This function tracks the provided object list within the provided frame.
	* @param: fishList, the reference list of the tracking fishes.
	* @param: frame, the reference image, which shall contain fishes.
	* @return: void.
	*/
	virtual void track(std::vector<TrackedObject> objectList, cv::Mat frame) = 0;

	/**
	* Resets the tracker.
	* @return: void.
	*/
	virtual void reset() = 0;

protected:
	Rectification &_rectification;
	Settings &_settings;

};


#endif // !TrackingAlgorithm_H
