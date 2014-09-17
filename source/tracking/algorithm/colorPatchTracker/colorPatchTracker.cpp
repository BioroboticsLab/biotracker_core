#include "colorPatchTracker.h"

ColorPatchTracker::ColorPatchTracker(Settings& settings) : TrackingAlgorithm(settings)
{
}


ColorPatchTracker::~ColorPatchTracker(void)
{
}
void ColorPatchTracker::track(std::vector<TrackedObject> &, ulong, cv::Mat &){}
void ColorPatchTracker::paint		( cv::Mat& ){}
void ColorPatchTracker::reset		(){}

void ColorPatchTracker::mouseMoveEvent		( QMouseEvent * )
{
}
void ColorPatchTracker::mousePressEvent		( QMouseEvent * )
{

}
void ColorPatchTracker::mouseReleaseEvent	( QMouseEvent * )
{	
}
