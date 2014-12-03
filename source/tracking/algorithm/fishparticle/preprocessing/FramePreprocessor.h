#ifndef FRAME_PREPROCESSOR_H
#define FRAME_PREPROCESSOR_H

#include <opencv2/opencv.hpp>

#include "FishBackgroundSubtractor.h"
#include "tracking/algorithm/fishparticle/particlefilter/particleParams.h"

/**
 * Does the preprocessing required for a frame of the video, so that the
 * particle system can work on the data. 
 */
class FramePreprocessor
{
public:
	explicit FramePreprocessor(Settings& settings, ParticleParams& params);
	virtual ~FramePreprocessor(void);
	cv::Mat preProcess(cv::Mat image);
	void reset();

private:
	/**
	 * The app's settings, containing settings for this instance.
	 */
	Settings& _settings;
	ParticleParams& _params;

	/**
	 * The background subtractor used to do the major work.
	 */
	FishBackgroundSubtractor _background_subtractor;

	cv::Mat backgroundSubtract(cv::Mat image);
};

#endif
