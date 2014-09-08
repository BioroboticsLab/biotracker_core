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

void ColorPatchTracker::mouseMoveEvent		( QMouseEvent * e )
{
}
void ColorPatchTracker::mousePressEvent		( QMouseEvent * e )
{

}
void ColorPatchTracker::mouseReleaseEvent	( QMouseEvent * e )
{	
}