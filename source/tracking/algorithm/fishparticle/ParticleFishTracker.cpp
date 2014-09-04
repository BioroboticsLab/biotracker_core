#include "ParticleFishTracker.h"

#include <opencv.hpp>

ParticleFishTracker::ParticleFishTracker(Settings& settings) : TrackingAlgorithm(settings), _preprocessor(settings)
{
}

ParticleFishTracker::~ParticleFishTracker(void)
{
}

void ParticleFishTracker::track(std::vector<TrackedObject>& objectList, unsigned long frameNumber, cv::Mat& frame) {
	_preprocessor.preProcess(frame);
	// TODO Rest
}

void ParticleFishTracker::paint(cv::Mat& image) {
	// TODO paint
}

void ParticleFishTracker::reset() {
	_preprocessor.reset();
	// TODO reset more
}
