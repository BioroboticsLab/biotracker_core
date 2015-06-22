#include "source/algorithm/SampleTracker/SampleObject.h"

SampleObject::SampleObject(void)
	: ObjectModel()
{
}


SampleObject::~SampleObject(void)
{
}

void SampleObject::setPosition(cv::Point){}
cv::Point SampleObject::getPosition(){return _position;}


