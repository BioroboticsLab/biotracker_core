#pragma once

#include "source/settings/Settings.h"
#include "source/tracking/Rectification.h"
#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/pose/FishPose.h"


template <typename T>
class IDetector
{
public:

	/**
	* The contructor with parameters.
	* @param: rectification, the rectification object for calculation the real world coordinate.
	* @param: systemProperty, the set of parameters for tracking property.
	*/
	IDetector(Rectification &rectification, Settings &settings) :
		_rectification(rectification),	_settings(settings) {}

	/**
	* The standard abtract destructor.
	*/
	virtual ~IDetector(void) {}

	/**
	* Interface function to get the fish poses from a binarzied image.
	* @param: binarized_image_mat, binarized image.
	* @return: a list of fish poses.
	*/
	virtual std::vector<T> getPoses(cv::Mat binarized_image_mat) = 0;

protected:
	Rectification &_rectification;
	Settings &_settings;
};
