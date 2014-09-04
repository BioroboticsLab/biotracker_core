#include "FramePreprocessor.h"


FramePreprocessor::FramePreprocessor(Settings& settings) : _settings(settings), _background_subtractor()
{
}


FramePreprocessor::~FramePreprocessor(void)
{
}

cv::Mat FramePreprocessor::preProcess(cv::Mat image) {
	image = backgroundSubtract(image);
	// cv::circle(image, cv::Point(50,50), 10, cv::Scalar(0, 0, 255), -1);
	return image;
}

cv::Mat FramePreprocessor::backgroundSubtract(cv::Mat image) {
	cv::Mat foreground;
	_background_subtractor(image, foreground);
	return foreground;
}

void FramePreprocessor::reset() {

}