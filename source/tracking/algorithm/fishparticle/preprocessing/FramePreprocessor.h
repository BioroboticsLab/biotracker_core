#ifndef FRAME_PREPROCESSOR_H
#define FRAME_PREPROCESSOR_H

#include <opencv.hpp>

#include "source/settings/Settings.h"

class FramePreprocessor
{
public:
	FramePreprocessor(Settings& settings);
	virtual ~FramePreprocessor(void);
	cv::Mat preProcess(cv::Mat image);
	void reset();

private:
	Settings& _settings;
	cv::BackgroundSubtractorMOG2 _background_subtractor;
	cv::Mat backgroundSubtract(cv::Mat image);
};

#endif
