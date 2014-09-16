#ifndef FISH_BACKGROUND_SUBTRACTOR_H_
#define FISH_BACKGROUND_SUBTRACTOR_H_

#include <opencv2/opencv.hpp>

#include "source/settings/Settings.h"

/**
* Doing the background subtraction, a configured cv::BackgroundSubtractorMOG2.
* Shadows are marked black instead of gray.
*/
class FishBackgroundSubtractor :
	public cv::BackgroundSubtractorMOG2
{
public:
	explicit FishBackgroundSubtractor(Settings& settings);
	~FishBackgroundSubtractor(void);
	void reset();

private:
	/**
	* The settings containing properties concerning this background subtractor.
	*/
	Settings& _settings;
};

#endif
