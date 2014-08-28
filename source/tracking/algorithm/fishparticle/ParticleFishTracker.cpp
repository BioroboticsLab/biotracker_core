#include "ParticleFishTracker.h"

#include <opencv.hpp>

ParticleFishTracker::ParticleFishTracker(Settings& settings) : TrackingAlgorithm(settings)
{
}


ParticleFishTracker::~ParticleFishTracker(void)
{
}

cv::Mat ParticleFishTracker::track(std::vector<TrackedObject>& objectList, unsigned long frameNumber, cv::Mat frame) {
	cv::circle(frame, cv::Point(50,50), 50, cv::Scalar(0, 255, 0), -1);
	return frame;
}

void ParticleFishTracker::reset() {
	// do nothing ... for now
}
