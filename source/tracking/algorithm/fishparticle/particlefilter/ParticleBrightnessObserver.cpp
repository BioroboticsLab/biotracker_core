#include "ParticleBrightnessObserver.h"


ParticleBrightnessObserver::ParticleBrightnessObserver(const cv::Mat image) : _image(image)
{
}


ParticleBrightnessObserver::~ParticleBrightnessObserver(void)
{
}

void ParticleBrightnessObserver::score(Particle& particle) {
	//int score = _image.at(cv::Point(particle.getX(), particle.getY()));
	//particle.setScore(score);
}
