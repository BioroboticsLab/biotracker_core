#include "ParticleBrightnessObserver.h"

/**
* Constructs a new ParticleBrightnessObserver, using the grayscale image for
* particle scores.
*/
ParticleBrightnessObserver::ParticleBrightnessObserver(cv::Mat image, ParticleParams& params) : _image(image), _params(params)
{
}


ParticleBrightnessObserver::~ParticleBrightnessObserver(void)
{
}

/**
* Scores a particle based on the image. Sets this particle's score according to
* the result. A particle's score is proportional to the value of the image at
* its position.
*/
void ParticleBrightnessObserver::score(Particle& particle) {
	uchar pixel = _image.at<uchar>(particle.getY(), particle.getX());
	
	particle.setScore(std::max(pixel, static_cast<uchar>(_params.getMinScore())));
}
