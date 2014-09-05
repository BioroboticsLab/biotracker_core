#ifndef COMPLEX_TRACKER_H_
#define COMPLEX_TRACKER_H_

#include "source/tracking/TrackingAlgorithm.h"
#include "preprocessing/FramePreprocessor.h"
#include "particlefilter/Particle.h"

/**
 * Implements a particle filtering algorithm to track multiple fish in a tank.
 */
class ParticleFishTracker :
	public TrackingAlgorithm
{
public:
	explicit ParticleFishTracker(Settings& settings);
	virtual ~ParticleFishTracker(void);
	virtual void track(std::vector<TrackedObject>& objectList, ulong frameNumber, cv::Mat& frame);
	virtual void paint(cv::Mat& image);
	virtual void reset();

private:
	/**
	 * Used to preprocess the image (mainly background subtraction).
	 */
	FramePreprocessor _preprocessor;

	/**
	 * The frame to be drawn the next time the GUI requests one.
	 */
	cv::Mat _prepared_frame;

	/**
	* The particles we are currently dealing with.
	*/
	std::vector<Particle> _current_particles;

	/**
	* The RNG used to generate random data for placing new particles.
	*/
	cv::RNG _rng;

	void seedParticles(unsigned num_particles, int min_x, int min_y, int max_x, int max_y);
};

#endif
