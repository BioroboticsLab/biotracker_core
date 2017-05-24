#pragma once

#include "Model/TrackingAlgorithm/imageProcessor/detector/IDetector.h"

#include "Model/TrackingAlgorithm/imageProcessor/detector/blob/BlobPose.h"
#include "Model/TrackingAlgorithm/imageProcessor/cvblobs/BlobResult.h"

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"

class SimpleBlobsDetector : public IDetector<BlobPose>
{
public:

	/**
	 * The contructor with parameters.
	 */
	SimpleBlobsDetector(void);

	virtual ~SimpleBlobsDetector(void) {}

	void setMask(cv::Mat *mask) { _mask = mask; }
		
	std::vector<BlobPose> getPoses(cv::Mat& binImage, cv::Mat& oriImage);

	void setDouble(std::string spec_param, double value);

private:

	cv::Mat *_mask;
	cv::SimpleBlobDetector::Params _params;

	/**
	 * Initialized the parameter for setting up the blob detector.
	 * @return: void. 
	 */
	void initParams();

	/**
	 * Find all blobs within an image.
	 * @param: binarized_image_mat, image contains blobs and is already binarized,
	 * @return: all found blobs within the image.
	 */
	
	// TODO: can we make binImage a reference??
	std::vector<BlobPose> findBlobs(const cv::Mat& binImage, const cv::Mat& oriImage);

	
};
