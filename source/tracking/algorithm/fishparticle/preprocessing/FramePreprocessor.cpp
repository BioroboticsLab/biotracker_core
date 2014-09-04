#include "FramePreprocessor.h"

// TODO parameter BG-Sub aus settings.
FramePreprocessor::FramePreprocessor(Settings& settings) : _settings(settings), _background_subtractor(cv::BackgroundSubtractorMOG2(30, 16))
{
}

FramePreprocessor::~FramePreprocessor(void)
{
}

cv::Mat FramePreprocessor::preProcess(cv::Mat image) {
	image = backgroundSubtract(image);
	return image;
}

cv::Mat FramePreprocessor::backgroundSubtract(cv::Mat image) {
	cv::Mat foreground;
	_background_subtractor(image, foreground);
	return foreground;
}

void FramePreprocessor::reset() {
	_background_subtractor = cv::BackgroundSubtractorMOG2();
}