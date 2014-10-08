#include "colorPatchTracker.h"

ColorPatchTracker::ColorPatchTracker(Settings& settings, QWidget *parent) : TrackingAlgorithm(settings, parent)
{
}


ColorPatchTracker::~ColorPatchTracker(void)
{
}
void ColorPatchTracker::track(ulong, cv::Mat &){}
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

void ColorPatchTracker::mouseWheelEvent	( QWheelEvent * ) {}