#ifndef SimpleTracker_H
#define SimpleTracker_H

#include <opencv.hpp>

#include "tracking/TrackingAlgorithm.h"
#include "TrackedFish.h"

class SimpleTracker :
	public TrackingAlgorithm
{
public:
	SimpleTracker(Settings& settings);
	~SimpleTracker(void);
	cv::Mat track(std::vector<TrackedObject> objectList, cv::Mat frame);
	void reset();

private:
	cv::BackgroundSubtractorMOG2 _bg_subtractor;
};

#endif