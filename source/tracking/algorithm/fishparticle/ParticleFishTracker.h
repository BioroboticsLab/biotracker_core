#ifndef COMPLEX_TRACKER_H_
#define COMPLEX_TRACKER_H_

#include "source/tracking/TrackingAlgorithm.h"

class ParticleFishTracker :
	public TrackingAlgorithm
{
public:
	ParticleFishTracker(Settings& settings);
	virtual ~ParticleFishTracker(void);
	virtual cv::Mat track(std::vector<TrackedObject>& objectList, unsigned long frameNumber, cv::Mat frame);
	virtual void reset();
};

#endif
