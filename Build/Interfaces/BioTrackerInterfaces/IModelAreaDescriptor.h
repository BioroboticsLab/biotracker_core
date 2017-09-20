#pragma once
#include "Interfaces/IModel/IModel.h"
#include <cv.h>

class IModelAreaDescriptor : public IModel
{
	Q_OBJECT

public:
	/**
	* The constructor of the IModelTrackedComponent class is able to receive a QObject as parent.
	*/
	IModelAreaDescriptor(QObject *parent = 0);

	virtual bool inTrackingArea(cv::Point2f p) = 0;

	//virtual std::vector<cv::Point2f> getRectificationCorners() = 0;

	virtual cv::Point2f pxToCm(cv::Point p) = 0;
	virtual cv::Point2f cmToPx(cv::Point2f p) = 0;

	//Gets the arena width in cm
	virtual double getWidth() = 0;

	//Gets the arena height in cm
	virtual double getHeight() = 0;


	/**
	* The methode operate() must be implemented by every derivative of this class.
	*/
	virtual void operate() = 0;
};
