#include "FramePreprocessor.h"


/**
 * Constructs a new instance using the preprocessor settings set in settings.
 */
FramePreprocessor::FramePreprocessor(Settings& settings, ParticleParams& params) : _settings(settings), _background_subtractor(settings), _params(params)
{
}

FramePreprocessor::~FramePreprocessor(void)
{
}

/**
 * This does the work. Preprocesses the image and returns the preprocessed Mat.
 * image may be modified by this method.
 */
cv::Mat FramePreprocessor::preProcess(cv::Mat image) {
	image = backgroundSubtract(image);
	/*cv::erode(image, image, cv::Mat());
	cv::dilate(image, image, cv::Mat());*/
	cv::GaussianBlur(image, image, cv::Size(_params.getGaussianBlurSize(), _params.getGaussianBlurSize()), 0);
	return image;
}

/**
 * Uses the background subtractor to get the foreground from the image. Does
 * not modify image.
 */
cv::Mat FramePreprocessor::backgroundSubtract(cv::Mat image) {
	cv::Mat foreground;
	_background_subtractor(image, foreground);
	return foreground;
}

/**
 * Since this object has state that is being accumulated while processing
 * frames from the video (ex background subtraction), this can be used to reset that state to the initial state.
 */
void FramePreprocessor::reset() {
	_background_subtractor.reset();
}