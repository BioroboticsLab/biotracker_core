#include "ParticleFishTracker.h"

#include <opencv.hpp>

#include "particlefilter/ParticleBrightnessObserver.h"

/**
* Constructs a new instance using the tracking and special particle tracker settings set in settings.
*/
ParticleFishTracker::ParticleFishTracker(Settings& settings) : TrackingAlgorithm(settings), _preprocessor(settings), _rng(123)
{
}

ParticleFishTracker::~ParticleFishTracker(void)
{
}

/**
* Does the main work, detecting tracked objects (fish) and building a history for those objects.
*/
void ParticleFishTracker::track(std::vector<TrackedObject>& objectList, unsigned long frameNumber, cv::Mat& frame) {
	// TODO check if frameNumber is jumping -> should lead to reseed

	// (1) Preprocess frame
	_prepared_frame = _preprocessor.preProcess(frame);
	// (2) Resampling (importance resampling) or seeding
	if (_current_particles.empty()) {
		// TODO params for this algorithm in settings.
		seedParticles(1000, 0, 0, frame.cols, frame.rows);
	} else {
		ParticleBrightnessObserver observer(_prepared_frame);
		for (Particle& p : _current_particles) {
			observer.score(p);
		}
		// TODO importance resample
	}
	// (3) Clustering (k-means)
	// TODO
	// (4) Store results in history
	// TODO
}

/**
* Fills the list of current particles (_current_particles) with num_particles
* uniformly distributed particles.
*/
void ParticleFishTracker::seedParticles(unsigned num_particles, int min_x, int min_y, int max_x, int max_y) {
	for (int i = 0; i<num_particles; i++) {
		int x = _rng.uniform(min_x, max_x);
		int y = _rng.uniform(min_y, max_y);
		// TODO include random angle
		float a = 0;

		Particle newParticle(x, y, a, _current_particles.size() + 1);
		_current_particles.push_back(newParticle);
	}
}

/**
* Draws the result of the tracking for the current frame.
*/
void ParticleFishTracker::paint(cv::Mat& image) {
	if (!_prepared_frame.empty()) {
		cv::cvtColor(_prepared_frame, image, CV_GRAY2BGR);
		for (const Particle& p : _current_particles) {
			cv::circle(image, cv::Point(p.getX(), p.getY()), 2, cv::Scalar(0, 255, 0), -1);
		}
	}
	// TODO separate drawing and tracking (eliminate _prepared_frame)
}

/**
* This algorithm has state, this can be used to reset that state. It will not
* delete the tracked objects' history.
*/
void ParticleFishTracker::reset() {
	_preprocessor.reset();
	// TODO reset more
}
