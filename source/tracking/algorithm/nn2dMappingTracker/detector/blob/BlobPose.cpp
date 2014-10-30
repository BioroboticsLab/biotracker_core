#include "BlobPose.h"

BlobPose::BlobPose(void)
{
}

BlobPose::BlobPose(cv::Point2f center_cm, cv::Point center_px, float angle_degree, float width, float height, bool use) :	
	_center_cm(center_cm),
	_center_px(center_px),
	_angle_degree(angle_degree),
	_width(width),
	_height(height),
	_used(use)
{}

BlobPose::~BlobPose(void)
{
}
