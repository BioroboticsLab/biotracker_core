#pragma once

#include "source/tracking/algorithm/nn2dMappingTracker/detector/IDetector.h"

#include "source/tracking/algorithm/nn2dMappingTracker/detector/blob/BlobPose.h"
#include "source/external/opencvblobslib/library/BlobResult.h"

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"

class SimpleBlobsDetector :
	public IDetector<BlobPose>
{
public:

	/**
	 * The contructor with parameters.
	 * @param: rectification, the rectification object for calculation the real world coordinate.
	 * @param: systemProperty, the set of parameters for tracking property.
	 */
	SimpleBlobsDetector(Rectification& rectification, Settings& settings);

	virtual ~SimpleBlobsDetector(void) {}

	std::vector<BlobPose> getPoses(cv::Mat binImage);
	void setMask(cv::Mat *mask) { _mask = mask; }

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
	std::vector<BlobPose> findBlobs(const cv::Mat& binImage);

	
};
