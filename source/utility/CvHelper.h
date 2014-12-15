#pragma once

#include <opencv2/opencv.hpp> // CV_PI, cv::Matx
#include <cmath>              // std::{sin,cos}
#include <stdexcept>          // std::invalid_argument

/**
 * Computer vision helper functions
 */
namespace CvHelper
{

	/**
	 * Convert degree to radian.
	 * @param deg degree value,
	 * @return rad value.
	 */
	inline float degToRad(float deg)
	{
		return deg * CV_PI / 180.0;
	}

	/**
	 * Convert radian to degree.
	 * @param rad radian value,
	 * @return degree value.
	 */
	inline float radToDeg(float rad)
	{
		return rad * 180.0 / CV_PI;
	}


	/**
	 * Roll.
	 * Performs a counterclockwise rotation of gamma about the x-axis.
	 *
	 * @see: http://planning.cs.uiuc.edu/node102.html
	 *
	 */
	inline cv::Matx<double, 3, 3> rotationMatrix_x(double gamma_rad) {
		using std::cos;
		using std::sin;

		// rotation angles:
		const double g = gamma_rad;  // angle to rotate around x axis

		return cv::Matx<double, 3, 3>
		(
		   1.0, /* | */ 0.0,    /* | */ 0.0,
		/*---------+---------------+------------*/
		   0.0, /* | */ cos(g), /* | */ -sin(g),
		/*---------+---------------+------------*/
		   0.0, /* | */ sin(g), /* | */ cos(g)
		);
	}


	/**
	 * Pitch.
	 * Performs a counterclockwise rotation of beta about the y-axis.
	 *
	 * @see: http://planning.cs.uiuc.edu/node102.html
	 *
	 */
	inline cv::Matx<double, 3, 3> rotationMatrix_y(double beta_rad) {
		using std::cos;
		using std::sin;

		// rotation angles:
		const double b = beta_rad;  // angle to rotate around x axis

		return cv::Matx<double, 3, 3>
		(
		   cos(b),  /* | */ 0.0, /* | */ sin(b),
		/*-------------+------------+-----------*/
		   0.0,     /* | */ 1.0, /* | */ 0.0,
		/*-------------+------------+-----------*/
		   -sin(b), /* | */ 0.0, /* | */ cos(b)
		);
	}


	/**
	 * Yaw.
	 * Performs a counterclockwise rotation of alpha about the z-axis.
	 *
	 * @see: http://planning.cs.uiuc.edu/node102.html
	 *
	 */
	inline cv::Matx<double, 3, 3> rotationMatrix_z(double alpha_rad) {
		using std::cos;
		using std::sin;

		// rotation angles:
		const double a = alpha_rad;  // angle to rotate around x axis

		return cv::Matx<double, 3, 3>
		(
		   cos(a), /* | */ -sin(a), /* | */ 0.0,
		/*------------+----------------+---------*/
		   sin(a), /* | */ cos(a),  /* | */ 0.0,
		/*------------+----------------+---- ----*/
		   0.0,    /* | */ 0.0,     /* | */ 1.0
		);
	}


	/**
	 * Roll-pitch-yaw.
	 * Rotates a point by roll_rad about the x-axis, then by pitch_rad about the y-axis and then by yaw_rad about the z-axis.
	 *
	 * rotationMatrix(yaw, pitch, roll) = rotationMatrix_z(yaw) * rotationMatrix_y(pitch) * rotationMatrix_x(roll)
	 *
	 * @see: http://planning.cs.uiuc.edu/node102.html
	 *
	 */
	inline cv::Matx<double, 3, 3> rotationMatrix(double yaw_rad, double pitch_rad, double roll_rad)
	{
		using std::cos;
		using std::sin;

		const double a = yaw_rad;   // angle to rotate around z axis
		const double b = pitch_rad; // angle to rotate around y axis
		const double g = roll_rad;  // angle to rotate around x axis

		return cv::Matx<double, 3, 3>
		(
		   cos(a)*cos(b), /* | */ cos(a)*sin(b)*sin(g) - sin(a)*cos(g), /* | */ cos(a)*sin(b)*cos(g) + sin(a)*sin(g),
		/*-------------------+---------------------------------------------+------------------------------------------*/
		   sin(a)*cos(b), /* | */ sin(a)*sin(b)*sin(g) + cos(a)*cos(g), /* | */ sin(a)*sin(b)*cos(g) - cos(a)*sin(g),
		/*-------------------+---------------------------------------------+------------------------------------------*/
		   -sin(b),       /* | */ cos(b)*sin(g),                        /* | */ cos(b)*cos(g)
		);
	}

	/**
	 *
	 * @param img            Image
	 * @param contours       All the input contours. Each contour is stored as a point vector.
	 * @param index_countour Parameter indicating a contour to draw
	 * @param color          Color of the contours
	 * @param close          Parameter indicating if the last and first vertex of the contour should be connected
	 * @param offset         Optional contour shift parameter.
	 */
	inline void drawPolyline(cv::Mat &img, std::vector<std::vector<cv::Point>> const &contours, size_t index_contour, cv::Scalar const & color, bool close = false, cv::Point offset = cv::Point())
	{
		if (contours.size() < 2) {
			throw std::invalid_argument("a contour contains a least 2 points");
		}
		for (size_t i = 1; i < contours[index_contour].size(); i++) {
			cv::line(img, offset + contours[index_contour][i - 1], offset + contours[index_contour][i], color);
		}
		if (close) {
			cv::line(img, offset + contours[index_contour].back(), offset + contours[index_contour].front(), color);
		}
	}

	template<typename POINT>
	inline double vecLength(POINT const & v)
	{	
		return sqrt(v.x * v.x + v.y * v.y);
	}
}
