#include "FishBackgroundSubtractor.h"

/**
* Performs the necessary application of settings to the underlying background
* subtractor.
*/
FishBackgroundSubtractor::FishBackgroundSubtractor(Settings& settings) : _settings(settings)
{
	nShadowDetection = 0;
}


FishBackgroundSubtractor::~FishBackgroundSubtractor(void)
{
}

/**
* Resets the state of this background subtractor.
*/
void FishBackgroundSubtractor::reset() {
	// TODO
}
