#ifndef PARTICLE_BRIGHTNESS_OBSERVER_
#define PARTICLE_BRIGHTNESS_OBSERVER_

#include <opencv2/opencv.hpp>
#include "tracking/algorithm/fishparticle/particlefilter/particleParams.h"

#include "Particle.h"

/**
* A simple 2D particle scoring algorithm. Uses the preprocessed frame, scoring
* a particle at position x, y based solely on the brightness of the pixel
* at x, y.
*/
class ParticleBrightnessObserver
{
public:
	ParticleBrightnessObserver(cv::Mat image, ParticleParams& params);
	~ParticleBrightnessObserver(void);
	void score(Particle& particle);
private:
	/**
	* The grayscale image used for scoring particles.
	*/
	cv::Mat _image;

	/**
	* The parameters used by this algorithm.
	*/
	ParticleParams& _params;
};

#endif
