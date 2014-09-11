#ifndef PARTICLE_BRIGHTNESS_OBSERVER_
#define PARTICLE_BRIGHTNESS_OBSERVER_

#include <opencv.hpp>

#include "Particle.h"

class ParticleBrightnessObserver
{
public:
	explicit ParticleBrightnessObserver(cv::Mat image);
	~ParticleBrightnessObserver(void);
	void score(Particle& particle);
private:
	/**
	* The grayscale image used for scoring particles.
	*/
	cv::Mat _image;
};

#endif
