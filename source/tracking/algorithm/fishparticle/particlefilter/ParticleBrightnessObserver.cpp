#include "ParticleBrightnessObserver.h"

/**
* Constructs a new ParticleBrightnessObserver, using the grayscale image for
* particle scores.
*/
ParticleBrightnessObserver::ParticleBrightnessObserver(cv::Mat image) : _image(image)
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
	
	// TODO min-Score from settings
	particle.setScore(std::max(pixel, static_cast<uchar>(10)));
}
