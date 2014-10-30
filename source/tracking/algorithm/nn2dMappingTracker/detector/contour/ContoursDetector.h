#pragma once

#include "source/tracking/algorithm/nn2dMappingTracker/detector/IDetector.h"

#include "source/tracking/algorithm/nn2dMappingTracker/detector/contour/ContourPose.h"

class ContoursDetector : public IDetector<ContourPose>
{
public:

	/**
	 * The contructor with parameters.
	 * @param: rectification, the rectification object for calculation the real world coordinate.
	 * @param: systemProperty, the set of parameters for tracking property.
	 */
	ContoursDetector(Rectification &rectification, Settings &setting) : 
		IDetector<ContourPose>(rectification,setting) {}

	~ContoursDetector(void) {}

	std::vector<ContourPose> getPoses(cv::Mat binarized_image_mat);

private:

	/**
	 * Find all cntours within an image.
	 * @param: binarized_image_mat, image contains blobs and is already binarized,
	 * @return: all found blobs within the image.
	 */
	std::vector<ContourPose> findContours(cv::Mat image_mat);

	void createMask(std::vector<cv::Point> points);

	cv::Mat _mask;
};
