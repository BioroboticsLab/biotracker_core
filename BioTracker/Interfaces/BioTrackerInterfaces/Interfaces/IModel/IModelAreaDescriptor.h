#pragma once

#include "IModel.h"
#include <string.h>
#include <cv.h>

class IModelAreaDescriptor :public IModel
{
	Q_OBJECT

public:
	IModelAreaDescriptor(QObject *parent = 0) : IModel(parent) {};
	//~IModelAreaDescriptor();
	
public:

	virtual bool inTrackingArea(cv::Point2f point_cm) = 0;

	/**
	* Transform the provided pixel coordinates into world coordinates and return world coordinates.
	* @param: pixelCoords, a list of points.
	* @return: world coordinates in as a list.
	*/
	virtual cv::Point2f pxToCm(cv::Point point_px) = 0;

	/**
	* Transform the provided pixel point into world coordinates and return world point.
	* @param: point, a pixel point, used opencv point
	* @return: world point.
	*/
	virtual cv::Point2f cmToPx(cv::Point2f point_cm) = 0;
public:

};

