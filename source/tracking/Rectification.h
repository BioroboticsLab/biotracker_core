#pragma once

#include <opencv2/opencv.hpp>

#include <QtCore/QList>
#include <QtCore/QPoint>

/**
 *	Rectification class normalizing the tracking image
 */
class Rectification
{
public:
	/**
	 * The standard constructor.
	 */
	Rectification(void);

	Rectification(double areaHeight_cm, double areaWidth_cm,
		std::vector<cv::Point> areaCoordinates, int camCaptureWidth_px,
		int camCaptureHeight_px);

	double areaWidth_cm() const { return _areaWidth_cm; }
	double areaHeight_cm() const { return _areaHeight_cm; }
	int captureWidth_px() const { return _camCaptureWidth_px; }
	int captureHeight_px() const { return _camCaptureHeight_px; }

	bool inArea(cv::Point2f point_cm) const;

	/**
	 * Transform the provided pixel coordinates into world coordinates and return world coordinates.
	 * @param point_px the provided pixel coordinates
	 * @return world coordinates in as a list.
	 */
	cv::Point2f pxToCm(cv::Point point_px) const;
	cv::Point2f pxToCm(QPoint point_px) const;

	/**
	 * Transform the provided pixel point into world coordinates and return world point.
	 * @param point_cm a pixel point, used opencv point
	 * @return world point.
	 */
	cv::Point2f cmToPx(cv::Point2f point_cm) const;

	/**
	 * Sets the tank coordinates.
	 * @param areaCoordinates the coordinate list of the considered area.
	 */
	void setArea(std::vector<cv::Point> areaCoordinates) { _areaCoordinates = std::move(areaCoordinates); }

	/**
	 * Sets the real world dimension the tank.
	 * @param areaWidth_cm width of the tank in cm,
	 * @param areaHeight_cm height of the tank in cm,
	 */
	void setDimension(double areaWidth_cm, double areaHeight_cm) { _areaWidth_cm = areaWidth_cm; _areaHeight_cm = areaHeight_cm; }

	std::vector<cv::Point> area() const { return _areaCoordinates; }

	int minXPx() const;
	int maxXPx() const;
	int minYPx() const;
	int maxYPx() const;

private: 
	std::vector<cv::Point> _areaCoordinates;
	std::vector<cv::Point2f> _rectifiedAreaCoordinates;

	cv::Mat_<double> _H;
	cv::Mat_<double> _H_inv;
	mutable cv::Mat_<double> _q;	

	int _camCaptureWidth_px;
	int _camCaptureHeight_px;

	double _areaWidth_cm;
	double _areaHeight_cm;
	
};
