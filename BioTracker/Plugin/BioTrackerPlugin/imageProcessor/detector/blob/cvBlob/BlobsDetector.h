#pragma once

#include "imageProcessor/detector/IDetector.h"

#include "imageProcessor/detector/blob/BlobPose.h"
#include "imageProcessor/cvblobs/BlobResult.h"

class BlobsDetector : public IDetector<BlobPose>
{
public:

	/**
	 * The contructor with parameters.
	 * @param: rectification, the rectification object for calculation the real world coordinate.
	 * @param: systemProperty, the set of parameters for tracking property.
	 */
	BlobsDetector(void);

	virtual ~BlobsDetector(void) {}

	void setMask(cv::Mat *mask) { _mask = mask; }

	std::vector<BlobPose> getPoses(cv::Mat& binImage, cv::Mat& oriImage);
	

	void setDouble(std::string spec_param, double value);

	double minBlobSize() { return _minBlobSize; };
	double maxBlobSize() { return _maxBlobSize; };
	void setMinBlobSize(double x) { _minBlobSize = x; };
	void setMaxBlobSize(double x) { _maxBlobSize = x; };

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
	std::vector<BlobPose> findBlobs(const cv::Mat& binImage, const cv::Mat& oriImage);

	double _minBlobSize;
	double _maxBlobSize;

	cv::Mat *_mask;
};
