#pragma once
#include "Interfaces/IModel/IModelAreaDescriptor.h"
#include "Interfaces/IModel/IModel.h"
#include "AreaInfoElement.h"

class AreaInfo : public IModelAreaDescriptor
{
	Q_OBJECT
public:
	AreaInfo(QObject *parent = 0);

	//TODO PORT
	bool inTrackingArea(cv::Point2f point_cm) override { return false; }; 

	/**
	* Transform the provided pixel coordinates into world coordinates and return world coordinates.
	* @param: pixelCoords, a list of points.
	* @return: world coordinates in as a list.
	*/
	cv::Point2f pxToCm(cv::Point point_px) override { return point_px; };

	/**
	* Transform the provided pixel point into world coordinates and return world point.
	* @param: point, a pixel point, used opencv point
	* @return: world point.
	*/
	cv::Point2f cmToPx(cv::Point2f point_cm) override { return point_cm; };

	std::shared_ptr<AreaInfoElement> _rect;
	std::shared_ptr<AreaInfoElement> _apperture;

private:



};

