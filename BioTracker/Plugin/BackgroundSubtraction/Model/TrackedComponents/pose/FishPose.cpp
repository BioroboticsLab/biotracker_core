#include "FishPose.h"
#include "helper/CvHelper.h"

FishPose::FishPose(cv::Point2f pos_cm , cv::Point pos_px, float rad, float deg, float width, float height, float score) :	
	_position_cm(pos_cm),
	_position_px(pos_px),
	_radAngle(rad),
	_degAngle(deg),
	_width(width),
	_height(height),
	_score(score)
{
	assert(_degAngle >= -360.0f && _degAngle <= 360.0f);
	time(&_timev);
}


std::string FishPose::toString(bool rectified)
{
	std::ostringstream out;
	if(rectified == false)
		out << _position_px.x << ";" << _position_px.y << ";" << _degAngle << ";" << _radAngle;
	else
		out << _position_cm.x << ";" << _position_cm.y << ";" << _degAngle << ";" << _radAngle;
	return out.str();
}

float FishPose::calculateProbabilityOfIdentity(const FishPose &first, const FishPose &second, float angleImportance)
{
    //Preliminary method for calculating identity of entities. See Issue biotracker_core/issues/130 for a discussion of this topic.
	float distance = CvHelper::getDistance(first.position_cm(), second.position_cm());
    return distance <= 0 ? std::numeric_limits<float>::max() : 1.0/distance;
}
