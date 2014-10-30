#pragma once

#include "source/tracking/algorithm/nn2dMappingTracker/detector/IDetector.h"

#include "source/tracking/algorithm/nn2dMappingTracker/detector/blob/BlobPose.h"
#include "source/external/cvblobs/BlobResult.h"

class BlobsDetector :
	public IDetector<BlobPose>
{
public:

	/**
	 * The contructor with parameters.
	 * @param: rectification, the rectification object for calculation the real world coordinate.
	 * @param: systemProperty, the set of parameters for tracking property.
	 */
	BlobsDetector(Rectification& rectification, Settings& settings);

	virtual ~BlobsDetector(void) {}

	std::vector<BlobPose> getPoses(cv::Mat binImage);
	void setMask(cv::Mat *mask) { _mask = mask; }

private:

	/**
	 * Removes blobs, which are too big for the further hanlding.
	 * @param: blobs, all possible blobs with all possible size.
	 * @return: void. 
	 */
	void filterBlobsBySize(CBlobResult& blobs);

	/**
	 * Find all blobs within an image.
	 * @param: binarized_image_mat, image contains blobs and is already binarized,
	 * @return: all found blobs within the image.
	 */
	
	// TODO: can we make binImage a reference??
	std::vector<BlobPose> findBlobs(const cv::Mat& binImage);

	cv::Mat *_mask;
};
