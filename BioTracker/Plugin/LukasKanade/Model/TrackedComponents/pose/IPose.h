#pragma once

#include <opencv2/opencv.hpp>

class IPose
{
public:
	virtual ~IPose() {}

	/**
	 * Gets the fish position as centimeter metric.
	 */
	virtual cv::Point2f position_cm() const = 0;

	/**
	 * Gets the fish position as pixel metric.
	 */
	virtual cv::Point position_px() const = 0;

	/**
	 * Gets the fish orientation in degree.
	 */
	virtual float orientation_deg() const = 0;

	/**
	 * Gets the fish orientation in radian.
	 */
	virtual float orientation_rad() const = 0;
};
