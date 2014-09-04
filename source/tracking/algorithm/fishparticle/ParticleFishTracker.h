#ifndef COMPLEX_TRACKER_H_
#define COMPLEX_TRACKER_H_

#include "source/tracking/TrackingAlgorithm.h"
#include "preprocessing/FramePreprocessor.h"

class ParticleFishTracker :
	public TrackingAlgorithm
{
public:
	ParticleFishTracker(Settings& settings);
	virtual ~ParticleFishTracker(void);
	virtual void track(std::vector<TrackedObject>& objectList, ulong frameNumber, cv::Mat& frame);
	virtual void paint(cv::Mat& image);
	virtual void reset();

private:
	FramePreprocessor _preprocessor;
};

#endif
