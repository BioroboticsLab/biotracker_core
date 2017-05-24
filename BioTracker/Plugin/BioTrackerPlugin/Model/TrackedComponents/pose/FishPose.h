#pragma once

#include "Model\TrackedComponents\pose\IPose.h"
#include <time.h>
#include <opencv2/opencv.hpp>

class FishPose : public IPose
{
public:
	/**
	 * The standard constructor.
	 */
	FishPose(cv::Point2f pos_cm = cv::Point2f(-1,-1), cv::Point pos_px = cv::Point(-1,-1), float rad = 0, float deg = 0, float width = 0, float height = 0, float score = 0.0);

	/**
	* Copy constructor.
	*/
	FishPose(const FishPose &other) : FishPose(other._position_cm, other._position_px, other._radAngle, other._degAngle, other._width, other._height, other._score)
	{
		time(&_timev);
	}

	/* Simple identity of fish poses, based on orientation, position and dimensions
 	*/
	inline bool operator == (const FishPose &b) const{
		if (b.orientation_rad() == orientation_rad() &&
			b.position_cm() == position_cm() &&
			b.width() == width() &&
			b.height() == height())
			return true;
		return false;
	}

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
	* Gets the position of the pose in pixel as floating point values.
	* @return: position in px.
	*/
	cv::Point2f	position_pxf() const { return cv::Point2f(_position_px.x, _position_px.y); }


	/**
	 * Gets the orientation of the pose as degree.
	 * @return: orientation angle.
	 */
	float		orientation_deg() const { return _degAngle; }

	/**
	* Sets the orientation of the pose as degree.
	* @param: degAngle, the orientation angle to set.
	*/
	void		set_orientation_deg(float degAngle) { _degAngle = degAngle; time(&_timev); assert(_degAngle >= -360.0f && _degAngle <= 360.0f); }

	/**
	 * Gets the orientation of the pose as radian.
	 * @return: orientation angle.
	 */
	float		orientation_rad() const { return _radAngle; }

	/**
	* Sets the orientation of the pose as radian.
	* @param: radAngle, the orientation angle to set.
	*/
	void		set_orientation_rad(float radAngle) { _radAngle = radAngle; time(&_timev);  }

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
	bool		isValid() { return (_position_px.x != -1 && _position_px.y != -1) ? true : false; }

	/**
	* @return: the score of the fish that comes from the mapper - usually between 0 and 1
	*/
	float		getScore() { return _score; }

	/**
	* Sets the score of the fish pose. Conventionally, it should be between 0 and 1.
	*/
	void		setScore(float score) { _score = score; }
	/**
	 * Gets the time stamp.
	 *
	 */
	time_t		ts() { return _timev; }

	/**
	 * Converts a pose to standard string.
	 * @param: rectified, specified whether the position is rectified.
	 * @return: a position as a standard string.
	 */
	std::string toString(bool rectified = false);

	/**
	* Calculates the probability that two poses are the same.
	* The influence of the angle can be set by the parameter angleImportance.
	* @return probability between 0 and 1
	*/
	static float calculateProbabilityOfIdentity(const FishPose &first, const FishPose &second, float angleImportance = 0.2f);

private:
	cv::Point2f _position_cm;
	cv::Point _position_px;
	float _radAngle;
	float _degAngle;
	float _width;
	float _height;
	float _score;
	time_t  _timev;
};
