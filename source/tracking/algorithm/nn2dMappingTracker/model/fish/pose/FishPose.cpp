#include "FishPose.h"


FishPose::FishPose(cv::Point2f pos_cm , cv::Point pos_px, float rad, float deg, float width, float height) :	
	_position_cm(pos_cm),
	_position_px(pos_px),
	_radAngle(rad),
	_degAngle(deg),
	_width(width),
	_height(height)
{}


std::string FishPose::toString(bool rectified)
{
	std::ostringstream out;
	if(rectified == false)
		//out << _position_px.x << "," << _position_px.y << "," << _degAngle << "," << _radAngle;
		out << _position_px.x << ";" << _position_px.y << ";" << _degAngle << ";" << _radAngle;
	else
		//out << _position_cm.x << "," << _position_cm.y << "," << _degAngle << "," << _radAngle;
		out << _position_cm.x << ";" << _position_cm.y << ";" << _degAngle << ";" << _radAngle;
	return out.str();
}
