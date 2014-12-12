#pragma once

#include <opencv2/opencv.hpp> // CV_PI, cv::Matx
#include <cmath>              // std::{sin,cos}

/**
 * Computer vision helper functions
 */
namespace CvHelper
{

	/**
	 * Convert degree to radian.
	 * @param deg, degree value,
	 * @return rad value.
	 */
	inline float degToRad(float deg)
	{
		return deg * CV_PI / 180.0;
	}

	/**
	 * Convert radian to degree.
	 * @param rad, radian value,
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
	 * @param gamma_rad
	 * @return
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
	 * @param beta_rad
	 * @return
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
	 * @param alpha_rad
	 * @return
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
	 * @param roll_rad
	 * @param pitch_rad
	 * @param yaw_rad
	 * @return
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

}
