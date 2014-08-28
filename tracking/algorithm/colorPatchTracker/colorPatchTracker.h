#ifndef ColorPatchTracker_H
#define ColorPatchTracker_H

#include "cv.h"
#include "settings/Settings.h"
#include "tracking\trackingalgorithm.h"



class ColorPatchTracker :
	public TrackingAlgorithm
{

public:
	ColorPatchTracker();
	ColorPatchTracker(Settings& settings);
	~ColorPatchTracker(void);
	cv::Mat track(std::vector<TrackedObject>& objectList, unsigned long frameNumber, cv::Mat frame) = 0;
};

#endif
