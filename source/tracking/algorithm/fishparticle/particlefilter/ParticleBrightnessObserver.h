#ifndef PARTICLE_BRIGHTNESS_OBSERVER_
#define PARTICLE_BRIGHTNESS_OBSERVER_

#include <opencv.hpp>

#include "Particle.h"

class ParticleBrightnessObserver
{
public:
	explicit ParticleBrightnessObserver(const cv::Mat image);
	~ParticleBrightnessObserver(void);
	void score(Particle& particle);
private:
	const cv::Mat& _image;
};

#endif
