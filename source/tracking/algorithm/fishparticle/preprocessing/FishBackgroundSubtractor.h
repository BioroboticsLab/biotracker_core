#ifndef FISH_BACKGROUND_SUBTRACTOR_H_
#define FISH_BACKGROUND_SUBTRACTOR_H_

#include <opencv2/opencv.hpp>

#include "tracking/algorithm/fishparticle/particlefilter/particleParams.h"

/**
* Doing the background subtraction, a configured cv::BackgroundSubtractorMOG2.
* Shadows are marked black instead of gray.
*/
class FishBackgroundSubtractor :
	public cv::BackgroundSubtractorMOG2
{
public:
	explicit FishBackgroundSubtractor(ParticleParams& params);
	~FishBackgroundSubtractor(void);
	void reset();

private:
	/**
	* The settings containing properties concerning this background subtractor.
	*/
	ParticleParams& _params;
};

#endif
