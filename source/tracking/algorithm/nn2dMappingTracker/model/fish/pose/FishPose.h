#pragma once

#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/pose/IPose.h"

class FishPose : public IPose
{
public:
	/**
	 * The standard constructor.
	 */
	FishPose(cv::Point2f pos_cm = cv::Point2f(-1,-1), cv::Point pos_px = cv::Point(-1,-1), float rad = 0, float deg = 0, float width = 0, float height = 0);

	~FishPose() {};

	/**
	 * Gets the position of the pose in cm.
	 * @return: position in cm.
	 */
	cv::Point2f position_cm() const { return _position_cm; }

	/**
	 * Gets the position of the pose in pixel.
	 * @return: position in px.
	 */
	cv::Point	position_px() const { return _position_px; }

	/**
	 * Gets the orientation of the pose as degree.
	 * @return: orientation angle.
	 */
	float		orientation_deg() const { return _degAngle; }

	/**
	 * Gets the orientation of the pose as radian.
	 * @return: orientation angle.
	 */
	float		orientation_rad() const { return _radAngle; }

	/**
	 * Gets the width pose.
	 * @return: width of the pose
	 */
	float		width() const { return _width; }

	/**
	 * Gets the height pose.
	 * @return: height of the pose
	 */
	float		height() const { return _height; }

	/**
	 * Checks if the fish pose is valid. Means, x-pos != -1 and y-pose != -1.
	 * @return: true if position is valid, false otherwise.
	 */
	bool isValid() { return (_position_px.x != -1 && _position_px.y != -1) ? true : false; }

	/**
	 * Converts a pose to standard string.
	 * @param: rectified, specified whether the position is rectified.
	 * @return: a position as a standard string.
	 */
	std::string toString(bool rectified = false);

private:
	cv::Point2f _position_cm;
	cv::Point _position_px;
	float _radAngle;
	float _degAngle;
	float _width;
	float _height;
};
