#pragma once
#include "source/tracking/serialization/ObjectModel.h"
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
