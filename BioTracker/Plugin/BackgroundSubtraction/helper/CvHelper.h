#pragma once

#include <QtCore/qmath.h>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <QDebug>

#include <opencv2/opencv.hpp>


/**
 * Computer vision helper class, contains only static methods
 */
namespace CvHelper
{
	/**
	 * Subtract operation for two CvPoints.
	 * @return: the difference of the CvPoints. 
	 */
	CvPoint subtractTwoCvPoints(CvPoint a, CvPoint b);

	/**
	 * Add operation for two CvPoints.
	 * @return: the sum of the CvPoints. 
	 */
	CvPoint addTwoCvPoints(CvPoint a, CvPoint b);

	CvPoint multCvPoint(double scalar, CvPoint p);

	/**
	 * Normalizes a coordinate point.
	 * @param x, the x coordinate.
	 * @param y, the y coordinate.
	 * @return the normlized point as floating number.
	 */
	QPointF norm(double x, double y);

	/**
	 * Normalizes a point.
	 * @param p, point to normalize.
	 * @return the normalized point as floating number.
	 */
	QPointF norm(QPoint p);

	/**
	 * Normalizes a floating number point.
	 * @param p, point to normalize.
	 * @return the normalized point as floating number.
	 */
	QPointF norm(QPointF p);

	/**
	 * Calculates the distance between two coordinates.
	 * @param x1, x-coordinate of the source point.
	 * @param y1, y-coordinate of the source point.
	 * @param x2, x-coordinate of the destination point.
	 * @param y2, y-coordinate of the destination point.
	 * @return the distance between the provided coordinates as floating number.
	 */
	double getDistance(double x1, double y1, double x2, double y2);
	double getSqDistance(double x1, double y1, double x2, double y2);

	/**
	* Calculates the distance between two coordinates.
	* @param x1, x-coordinate of the source point.
	* @param y1, y-coordinate of the source point.
	* @param z1, z-coordinate of the source point.
	* @param x2, x-coordinate of the destination point.
	* @param y2, y-coordinate of the destination point.
	* @param z2, y-coordinate of the destination point.
	* @return the distance between the provided coordinates as floating number.
	*/
	double getDistance(double x1, double y1, double z1, double x2, double y2, double z2);
	double getSqDistance(double x1, double y1, double z1, double x2, double y2, double z2);

	/**
	 * Calculates the distance between two points.
	 * @param p1, source point.
	 * @param p2, destination point.
	 * @return the distance between p1 and p2 as floating number.
	 */
	double getDistance(QPoint p1, QPoint p2);

	/**
	 * Calculates the distance between two floating number points.
	 * @param p1, source point.
	 * @param p2, destination point.
	 * @return the distance between p1 and p2 as floating number.
	 */
	double getDistance(QPointF p1, QPointF p2);

	double getDistance(cv::Point2f p1, cv::Point2f p2);	
	double getDistance(cv::Point p1, cv::Point p2);
	double getSqDistance(cv::Point2f p1, cv::Point2f p2);

	double getDistance(cv::Point3f p1, cv::Point3f p2);
	double getSqDistance(cv::Point3f p1, cv::Point3f p2);

	double orientation(cv::Point2f front, cv::Point2f back);
	double orientation(QPointF front, QPointF back);

	float angleDifference(float alpha, float beta);

	/**
	 * Convert degree to radian.
	 * @param: deg, degree value,
	 * @return: rad value.
	 */
	float degToRad(float deg);

	/**
	 * Convert radian to degree.
	 * @param: rad, radian value,
	 * @return: degree value.
	 */
	float radToDeg(float rad);
	
	/**
	 * This function finds the minimal number of three numbers.
	 */
	float findMin(float n1, float n2, float n3);

	/**
	 * This function finds the maximal number of three numbers.
	 */
	float findMax(float n1, float n2, float n3);

	float normalDist(float x, float mean, float variance);

	float sigmoid(float x, float shrink);

	float sigmoidAbsInv(float x, float shrink);

	double getAngleDifference(double dirToTargetAsRad, double currOrientationAsRad);
	double getAngleToTarget(cv::Point2f A, cv::Point2f B);
	//double getAngleToTarget(cv::Point2f currentPos, cv::Point2f targetPos);	

	cv::Point2f getMirrowPoint(cv::Point2f point2Mirror, cv::Point2f pointOfOrigin, float angelAsGrad);
	std::deque<cv::Point2f> getMirrowPoints(std::deque<cv::Point2f> points2Mirror, cv::Point2f pointOfOrigin, float angelAsGrad);
	std::deque<cv::Point2f> getMirrowLine(cv::Point2f pointOfOrigin, float width, float height, float angelAsGrad);


	std::deque<cv::Point2f> convertMat2Point2fDeque(cv::Mat mat);
	cv::Mat convertPoint2fDeque2Mat(std::deque<cv::Point2f> points);
	QList<std::deque<cv::Point2f>> convertMatList2Point2fDequeList(QList<cv::Mat> mats);
	std::vector<cv::Point> convertMat2Vector(cv::Mat mat);
	cv::Mat convertVector2Mat(std::vector<cv::Point> vect);

	/**
	 * Converts a string to an integer number.
	 * @param: string, a string containing a number.
	 * @return: a converted integer number.
	 */ 
	int stdStringToInt(std::string string);

	/**
	 * Converts a string to a float number.
	 * @param: string, a string containing a number.
	 * @return: a converted float number.
	 */ 
	float stdStringToFloat(std::string string);

	/**
	 * Converts a list of cv::Point to a proper string, which could be save into the config.ini file.
	 * @param: points, a list of cv::Point.
	 * @return: a converted formated string for the config.ini file.
	 */ 
	std::string convertStdVectorCvPointToStdString(std::vector<cv::Point> points);

	/**
	 * Converts a cv::Scalar to a proper standard string, which could be save into the config.ini file.
	 * @param: scalar, a scalar.
	 * @return: a converted formated string for the config.ini file.
	 */ 
	std::string convertCvScalarToStdString(cv::Scalar scalar);

	/**
	 * Gets the current date time as std string.
	 * @return: the current formated date time as standard string.
	 */ 
	std::string getCurrentDatetimeAsStd();

	std::string cvSizeToSS(cv::Size cvSize_value);
	std::string cvSize2fToSS(cv::Size2f cvSize2f_value);
	std::string cvSize2fToSS(QString width, QString height);


	/**
	 * Checks if image sizes are equal.
	 * @param: size of first object
	 * @param: size of second object
	 * @return: true if equal, false otherwise
	 */
	bool isFrameSizeEqual(cv::Size o1, cv::Size o2);
}
