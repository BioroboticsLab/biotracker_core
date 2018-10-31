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

	static Rectification& instance()
	{
		static Rectification _instance(100,100); // Guaranteed to be destroyed.
		// Instantiated on first use.
		return _instance;
	}

	~Rectification() {}

private:

	/**
	* The standard constructor.
	*/
	Rectification(float w, float h);

	Rectification(const Rectification&); /* verhindert, dass eine weitere Instanz via
											Kopier-Konstruktor erstellt werden kann */

	Rectification & operator = (const Rectification &); //Verhindert weitere Instanz durch Kopie	

public:

	void init(double areaHeight_cm, double areaWidth_cm,
		std::vector<cv::Point> areaCoordinates, int camCaptureWidth_px,
		int camCaptureHeight_px, int frameDisplayWidthPx, int frameDisplayHeightPx);

	bool isSetup() { return _isSetup; };

	double areaWidth_cm() const { return _areaWidth_cm; }
	double areaHeight_cm() const { return _areaHeight_cm; }
	int captureWidth_px() const { return _camCaptureWidth_px; }
	int captureHeight_px() const { return _camCaptureHeight_px; }
	
	bool inArea(cv::Point2f point_cm) const;

	/**
	 * Transform the provided pixel coordinates into world coordinates and return world coordinates.
	 * @param: pixelCoords, a list of points.
	 * @return: world coordinates in as a list.
	 */
	cv::Point2f pxToCm(cv::Point point_px) const;
	cv::Point2f pxToCm(QPoint point_px) const;

	/**
	 * Transform the provided pixel point into world coordinates and return world point.
	 * @param: point, a pixel point, used opencv point
	 * @return: world point.
	 */
	cv::Point2f cmToPx(cv::Point2f point_cm) const;

	/**
	 * Sets the tank coordinates.
	 * @param: areaCoordinates, the coordinate list of the considered area.
	 * @return: void.
	 */
	void setArea(std::vector<cv::Point> areaCoordinates);
	void setArea(std::vector<QPoint> areaCoordinates);
	
	//infers the area coordinates from the data model
	void setArea();
	std::vector<cv::Point> getDefaultArena();
	void resetAreaCoordinates();

	/**
	 * Sets the real world dimension the tank.
	 * @param: areaWidth_cm, width of the tank in cm,
	 * @param: areaHeight_cm, height of the tank in cm,
	 * @return: void.
	 */
	void setDimension(double areaWidth_cm, double areaHeight_cm);

	void setCamImageSize(cv::Size size) { _camCaptureWidth_px = size.width; _camCaptureHeight_px = size.height; }

	void initRecitification(double areaHeight_cm, double areaWidth_cm);
	void setupRecitification(int frameDisplayWidthPx, int frameDisplayHeightPx, int camImageWidth, int camImageHeight);

	std::vector<cv::Point> area() const { return _areaCoordinates; }

	int minXPx();
	int maxXPx();
	int minYPx();
	int maxYPx();

	cv::Point getTopLeft();
	cv::Point getBottomLeft();
	cv::Point getBottomRight();
	cv::Point getTopRight();
	cv::Point getCorrectedResolutionPoint(QPoint pos);
	cv::Point getCorrectedResolutionPoint(cv::Point pos);
	cv::Mat   getSortedPoints();

private: 
	std::vector<cv::Point> _areaCoordinates;
	std::vector<cv::Point2f> _rectifiedAreaCoordinates;

	//Homography
	cv::Mat_<double> _H;
	cv::Mat_<double> _H_inv;
	//some vector 3x1. Might be zero? TODO
	mutable cv::Mat_<double> _q;	

	int _camCaptureWidth_px;
	int _camCaptureHeight_px;

	int _frameDisplayWidthPx;
	int _frameDisplayHeightPx;

	double _areaWidth_cm;
	double _areaHeight_cm;

	bool _isSetup;

	bool checkValidCoordinates(std::vector<cv::Point> areaCoordinates, int camCaptureHeight_px, int camCaptureWidth_px);
	
};
