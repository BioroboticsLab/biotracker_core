#pragma once

#include "Model/TrackingAlgorithm/imageProcessor/detector/IDetector.h"

#include "Model/TrackingAlgorithm/imageProcessor/detector/contour/ContourPose.h"

class ContoursDetector : public IDetector<ContourPose>
{
public:

	/**
	 * The contructor with parameters.
	 */
	ContoursDetector(void) {}

	~ContoursDetector(void) {}

	std::vector<ContourPose> getPoses(cv::Mat& binImage, cv::Mat& oriImage);

	void setDouble(std::string spec_param, double value);

private:

	/**
	 * Find all cntours within an image.
	 * @param: binarized_image_mat, image contains blobs and is already binarized,
	 * @return: all found blobs within the image.
	 */
	std::vector<ContourPose> findBlobs(const cv::Mat& binImage, const cv::Mat& oriImage);

	std::vector<ContourPose> findContours(const cv::Mat& binImage, const cv::Mat& oriImage);

	void createMask(std::vector<cv::Point> points);

	cv::Mat _mask;
};
