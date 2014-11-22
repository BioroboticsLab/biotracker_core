#ifndef PARTICLE_BRIGHTNESS_OBSERVER_
#define PARTICLE_BRIGHTNESS_OBSERVER_

#include <opencv2/opencv.hpp>
#include "tracking/algorithm/fishparticle/particlefilter/particleParams.h"

#include "Particle.h"

class ParticleBrightnessObserver
{
public:
	explicit ParticleBrightnessObserver(cv::Mat image, ParticleParams& params);
	~ParticleBrightnessObserver(void);
	void score(Particle& particle);
private:
	/**
	* The grayscale image used for scoring particles.
	*/
	cv::Mat _image;
	ParticleParams& _params;
};

#endif
