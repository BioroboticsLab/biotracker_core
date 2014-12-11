#pragma once

#include <opencv2/opencv.hpp>

/**
 * Computer vision helper class, contains only static methods
 */
namespace CvHelper
{

	/**
	 * Convert degree to radian.
	 * @param deg, degree value,
	 * @return rad value.
	 */
	inline float CvHelper::degToRad(float deg)
	{
		return deg * CV_PI / 180.0;
	}

	/**
	 * Convert radian to degree.
	 * @param rad, radian value,
	 * @return degree value.
	 */
	float CvHelper::radToDeg(float rad)
	{
		return rad * 180.0 / CV_PI;
	}
	
}
