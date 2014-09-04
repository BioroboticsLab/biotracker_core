#include "ParticleFishTracker.h"

#include <opencv.hpp>

#include "preprocessing/FramePreprocessor.h"

ParticleFishTracker::ParticleFishTracker(Settings& settings) : TrackingAlgorithm(settings)
{
}


ParticleFishTracker::~ParticleFishTracker(void)
{
}

cv::Mat ParticleFishTracker::track(std::vector<TrackedObject>& objectList, unsigned long frameNumber, cv::Mat frame) {
	FramePreprocessor preprocessor(_settings);
	cv::Mat image;
	frame = preprocessor.preProcess(frame);
	
	return image;
}

void ParticleFishTracker::reset() {
	// do nothing ... for now
}
