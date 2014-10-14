#pragma once
#include "source/tracking/trackedobject/objectmodel.h"
#include <cv.h>
class SampleObject :
	public ObjectModel
{
public:
	SampleObject(void);
	virtual ~SampleObject(void);
	void setPosition(cv::Point pos);
	cv::Point getPosition();

private:
	cv::Point _position;
};
