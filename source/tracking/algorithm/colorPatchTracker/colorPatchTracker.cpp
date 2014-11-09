#include "colorPatchTracker.h"

ColorPatchTracker::ColorPatchTracker(Settings& settings, std::string &serializationPathName, QWidget *parent)
    : TrackingAlgorithm(settings, serializationPathName, parent)
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
