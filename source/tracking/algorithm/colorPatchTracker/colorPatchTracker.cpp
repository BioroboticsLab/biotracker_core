#include "colorPatchTracker.h"

ColorPatchTracker::ColorPatchTracker(Settings& settings) : TrackingAlgorithm(settings)
{
}


ColorPatchTracker::~ColorPatchTracker(void)
{
}
void ColorPatchTracker::track(std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat & frame){}
void ColorPatchTracker::paint		( cv::Mat& image ){}
void ColorPatchTracker::reset		(){}