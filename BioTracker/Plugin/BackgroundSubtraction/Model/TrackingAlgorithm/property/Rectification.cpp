#include "Rectification.h"
#include <iostream>

#include <opencv2/calib3d/calib3d.hpp>
#include "settings/Settings.h"
#include "Model/TrackingAlgorithm/property/ParamNames.h"
#include "util/misc.h"

void Rectification::init(double areaHeight_cm, double areaWidth_cm,
		std::vector<cv::Point> areaCoordinates, int camCaptureWidth_px,
		int camCaptureHeight_px, int frameDisplayWidthPx, int frameDisplayHeightPx)	
{
	_q = cv::Mat(3, 1, CV_32F);
	_camCaptureWidth_px = camCaptureWidth_px;
	_camCaptureHeight_px = camCaptureHeight_px;
	_areaHeight_cm = areaHeight_cm;
	_areaWidth_cm = areaWidth_cm;
	_areaCoordinates = areaCoordinates;
	_frameDisplayWidthPx = frameDisplayWidthPx;
	_frameDisplayHeightPx = frameDisplayHeightPx;

	setupRecitification(frameDisplayWidthPx, frameDisplayHeightPx, camCaptureWidth_px, camCaptureHeight_px);
}

void Rectification::initRecitification(double areaHeight_cm, double areaWidth_cm, TrackedTrajectory* dmodel)//, std::vector<cv::Point> areaCoordinates
{
	//setArea(areaCoordinates);
	_dataModelM = dmodel;
	setArea();
	setDimension(areaWidth_cm, areaHeight_cm);
}


void Rectification::setArea(std::vector<cv::Point> areaCoordinates) {
	_areaCoordinates = areaCoordinates;
	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	_settings->setParam(TRACKERPARAM::CN_ARENA, cvPointsToString(areaCoordinates).c_str());
}
void Rectification::setArea(std::vector<QPoint> areaCoordinates) {
	std::vector<cv::Point> cvAreaCoordinates;
	for (int i = 0; i < areaCoordinates.size(); i++) {
		cvAreaCoordinates.push_back(cv::Point(areaCoordinates[i].x(), areaCoordinates[i].y()));
	}
	setArea(cvAreaCoordinates);
}

std::vector<cv::Point> Rectification::getDefaultArena() {
	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	std::string arena = _settings->getValueOrDefault<std::string>(TRACKERPARAM::CN_ARENA, "10,10;10,100;100,100;100,10");
	return stringToCVPointVec(arena);
}

void Rectification::setArea()
{
	setArea(getDefaultArena());
}

void Rectification::setupRecitification(int frameDisplayWidthPx, int frameDisplayHeightPx, int camImageWidth, int camImageHeight)
{
	_frameDisplayWidthPx = frameDisplayWidthPx;
	_frameDisplayHeightPx = frameDisplayHeightPx;

	_camCaptureWidth_px = camImageWidth;
	_camCaptureHeight_px = camImageHeight;

	if (_rectifiedAreaCoordinates.size() > 0) {
		_rectifiedAreaCoordinates.clear();
	}

	_rectifiedAreaCoordinates.push_back(cv::Point2f(0.0, 0.0)); // Top left
	_rectifiedAreaCoordinates.push_back(cv::Point2f(0.0, _areaHeight_cm)); // botton left
	_rectifiedAreaCoordinates.push_back(cv::Point2f(_areaWidth_cm, _areaHeight_cm)); // bottom right 
	_rectifiedAreaCoordinates.push_back(cv::Point2f(_areaWidth_cm, 0.0)); // top right

	std::vector<cv::Point2f> areaCoordinates_2f;

	assert(_areaCoordinates.size() == 4);

	cv::Mat coordinateMatrix(getSortedPoints());
	
	if (cv::sum(coordinateMatrix)[0] > 0.0f)
		coordinateMatrix.copyTo(areaCoordinates_2f);
	else
		areaCoordinates_2f = _rectifiedAreaCoordinates;

	_H = cv::findHomography(areaCoordinates_2f, _rectifiedAreaCoordinates);
	_H_inv = _H.inv();
	_isSetup = true;
}

void Rectification::resetAreaCoordinates()
{
	std::vector<cv::Point> areaCoordinates;

	double rationHeight = double(_frameDisplayHeightPx) / double(_camCaptureHeight_px);
	double rationWidth = double(_frameDisplayWidthPx) / double(_camCaptureWidth_px);

	for (cv::Point p : _areaCoordinates) {


		int newP_x = p.x / rationWidth;
		int newP_y = p.y / rationHeight;

		areaCoordinates.push_back(cv::Point(newP_x, newP_y));
	}

	if (checkValidCoordinates(areaCoordinates, _camCaptureHeight_px, _camCaptureWidth_px))
	{
		_areaCoordinates.clear();
		_areaCoordinates = areaCoordinates;

		setupRecitification(_frameDisplayWidthPx, _frameDisplayHeightPx, _camCaptureWidth_px, _camCaptureHeight_px);
	}
}

bool Rectification::checkValidCoordinates(std::vector<cv::Point> areaCoordinates, int camCaptureHeight_px, int camCaptureWidth_px)
{	
	for (cv::Point p : areaCoordinates) {
		if (p.y > camCaptureHeight_px)	return false;
		if (p.x > camCaptureWidth_px)	return false;		
	}

	return true;
}

cv::Point2f Rectification::pxToCm(cv::Point point_px) const
{
	cv::Mat_<double> v = (cv::Mat_<double>(3,1) << point_px.x, point_px.y, 1);
	cv::gemm(_H, v, 1, 0, 0, _q);
	double* qx = _q.ptr<double>(0);
	double* qy = _q.ptr<double>(1);
	double* qz = _q.ptr<double>(2);

	cv::Point2f cm((*qx / *qz), (*qy / *qz));

	//return cv::Point2f((*qx / *qz), (*qy / *qz));
	return cm;
}

cv::Point2f Rectification::pxToCm(QPoint point_px) const
{
	return pxToCm(cv::Point(point_px.x(), point_px.y()));
}

cv::Point2f Rectification::cmToPx(cv::Point2f point_cm) const
{
	cv::Mat_<double> v = (cv::Mat_<double>(3, 1) << point_cm.x, point_cm.y, 1);
	cv::gemm(_H_inv, v, 1, 0, 0, _q);
	double *qx = _q.ptr<double>(0);
	double *qy = _q.ptr<double>(1);
	double *qz = _q.ptr<double>(2);

	return cv::Point2f((*qx / *qz), (*qy / *qz));
}

bool Rectification::inArea(cv::Point2f point_cm) const
{	
	/*

	std::cout << "cur pos: " << point_cm << std::endl;

	double rationHeight = double(_frameDisplayHeightPx) / double(_camCaptureHeight_px);

	double widhtNew = rationHeight * _camCaptureWidth_px;
	
	int marginHorizontalPx = 0;
	int marginVerticalPx = 0;


	if (widhtNew <= _frameDisplayWidthPx)
	{
		marginHorizontalPx = (_frameDisplayWidthPx - int(widhtNew)) / 2;
		marginHorizontalPx /= rationHeight;

	} else {
		double rationWidth = double(widhtNew) / double(_camCaptureWidth_px);

		double heightNew = rationWidth * _camCaptureHeight_px;

		if (heightNew <= _frameDisplayHeightPx)
		{
			marginVerticalPx = (_frameDisplayHeightPx - int(heightNew)) / 2;
			marginVerticalPx /= rationWidth;
		}
	}

	double marginHorizontalCm = 0;
	double marginVerticalCm = 0;

	if (marginHorizontalPx > 0)
	{
		marginHorizontalCm = cv::abs(pxToCm(cv::Point(marginHorizontalPx, 0)).x);
	}

	if (marginVerticalPx > 0)
	{
		marginVerticalCm = cv::abs(pxToCm(cv::Point(0, marginVerticalPx)).y);
	}

	//return (point_cm.x >= 0 && point_cm.y >= 0	&& point_cm.x <= _areaWidth_cm + marginHorizontalCm	&& point_cm.y <= _areaHeight_cm + marginVerticalCm);
	bool flag = ((point_cm.x + (marginHorizontalCm / 2)) >= 0 
		&& (point_cm.y + (marginVerticalCm / 2)) >= 0 
		&& point_cm.x <= (_areaWidth_cm + (marginHorizontalCm / 2) )
		&& point_cm.y <= _areaHeight_cm + (marginVerticalCm / 2));
	
	std::cout << "inarea: " << flag << std::endl;

	return flag;*/

	bool flag = (point_cm.x >= 0 && point_cm.y >= 0 && point_cm.x <= _areaWidth_cm	&& point_cm.y <= _areaHeight_cm);

	return flag;
}


int Rectification::minXPx()
{
	int minX = _camCaptureWidth_px;
	for (int i = 0; i < _areaCoordinates.size(); i++)
	{
		if(_areaCoordinates.at(i).x < minX)
			minX = _areaCoordinates.at(i).x;
	}
	return minX;
}

int Rectification::maxXPx()
{
	int maxX = -1;
	for (int i = 0; i < _areaCoordinates.size(); i++)
	{
		if(_areaCoordinates.at(i).x > maxX)
			maxX = _areaCoordinates.at(i).x;
	}
	return maxX;
}

int Rectification::minYPx()
{
	int minY = _camCaptureHeight_px;
	for (int i = 0; i < _areaCoordinates.size(); i++)
	{
		if(_areaCoordinates.at(i).y < minY)
			minY = _areaCoordinates.at(i).y;
	}
	return minY;
}

int Rectification::maxYPx()
{
	int maxY = -1;
	for (int i = 0; i < _areaCoordinates.size(); i++)
	{
		if(_areaCoordinates.at(i).y > maxY)
			maxY = _areaCoordinates.at(i).y;
	}
	return maxY;
}

cv::Point Rectification::getTopLeft()
{
	assert(_areaCoordinates.size() == 4);

	std::vector<cv::Point> ac(_areaCoordinates);

	std::sort(ac.begin(), ac.end(),
		[](const cv::Point & a, const cv::Point & b) -> bool
	{
		return (a.x <= b.x) && (a.y <= b.y);
	}
	);	

	return ac[0];
}

cv::Point Rectification::getBottomLeft()
{
	assert(_areaCoordinates.size() == 4);

	std::vector<cv::Point> ac(_areaCoordinates);

	std::sort(ac.begin(), ac.end(),
		[](const cv::Point & a, const cv::Point & b) -> bool
	{
		return (a.x <= b.x) && (a.y >= b.y);
	}
	);

	return ac[0];
}

cv::Point Rectification::getBottomRight()
{
	assert(_areaCoordinates.size() == 4);

	std::vector<cv::Point> ac(_areaCoordinates);

	std::sort(ac.begin(), ac.end(),
		[](const cv::Point & a, const cv::Point & b) -> bool
	{
		return (a.x >= b.x) && (a.y >= b.y);
	}
	);


	return ac[0];
}

cv::Point Rectification::getTopRight()
{
	assert(_areaCoordinates.size() == 4);


	std::vector<cv::Point> ac(_areaCoordinates);

	std::sort(ac.begin(), ac.end(),
		[](const cv::Point & a, const cv::Point & b) -> bool
	{
		return (a.x >= b.x) && (a.y <= b.y);
	}
	);

	return ac[0];
}

cv::Mat Rectification::getSortedPoints()
{
	assert(_areaCoordinates.size() == 4);

	return cv::Mat(_areaCoordinates);

	/*
	std::vector<cv::Point> sortedCoordinates;

	//get top left
	std::vector<cv::Point> ac(_areaCoordinates);

	std::sort(ac.begin(), ac.end(),
		[](const cv::Point & a, const cv::Point & b) -> bool
	{
		return (a.x < b.x && a.y < b.y);
		
	}
	);

	sortedCoordinates.push_back(ac.front());
	ac.erase(ac.begin());	

	std::sort(ac.begin(), ac.end(),
		[](const cv::Point & a, const cv::Point & b) -> bool
	{
		return (a.x < b.x);
	}
	);

	sortedCoordinates.push_back(ac.front());
	ac.erase(ac.begin());

	std::sort(ac.begin(), ac.end(),
		[](const cv::Point & a, const cv::Point & b) -> bool
	{
		return (a.y >= b.y);
	}
	);

	sortedCoordinates.push_back(ac.front());
	ac.erase(ac.begin());

	sortedCoordinates.push_back(ac.front());

	return cv::Mat(sortedCoordinates);

	*/
}

cv::Point Rectification::getCorrectedResolutionPoint(QPoint pos)
{
	return getCorrectedResolutionPoint(cv::Point(pos.x(),pos.y()));
}

cv::Point Rectification::getCorrectedResolutionPoint(cv::Point pos)
{

	int heightScaled = _frameDisplayHeightPx;
	int widthScaled = _frameDisplayWidthPx;

	float scaleFactor = 1.0f;

	if (_camCaptureWidth_px > _frameDisplayWidthPx || _camCaptureHeight_px > _frameDisplayHeightPx)
	{
		scaleFactor = float(_camCaptureWidth_px) / float(_frameDisplayWidthPx);

		heightScaled = _camCaptureHeight_px / scaleFactor;

		if (_camCaptureWidth_px == _camCaptureHeight_px)
		{
			if (heightScaled > _frameDisplayHeightPx)
			{
				scaleFactor = float(_camCaptureHeight_px) / float(_frameDisplayHeightPx);

				widthScaled = _camCaptureWidth_px / scaleFactor;

				assert(widthScaled <= _frameDisplayWidthPx);
			}
			else
			{
				if (heightScaled > _frameDisplayWidthPx)
				{
					scaleFactor = float(_camCaptureHeight_px) / float(_frameDisplayWidthPx);

					widthScaled = _camCaptureWidth_px / scaleFactor;

					assert(widthScaled <= _frameDisplayWidthPx);
				}
			}
		}
		else
		{
			if (heightScaled > _frameDisplayWidthPx)
			{
				scaleFactor = float(_camCaptureHeight_px) / float(_frameDisplayWidthPx);

				widthScaled = _camCaptureWidth_px / scaleFactor;

				assert(widthScaled <= _frameDisplayWidthPx);
			}
		}

		
	}

	return cv::Point(int(float(pos.x) * scaleFactor), int(float(pos.y) * scaleFactor));
}