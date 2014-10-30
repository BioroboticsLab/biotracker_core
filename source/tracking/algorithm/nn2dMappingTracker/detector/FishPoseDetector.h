#pragma once

#include "source/tracking/algorithm/nn2dMappingTracker/detector/IDetector.h"

#include "source/tracking/algorithm/nn2dMappingTracker/detector/blob/BlobPose.h"
#include "source/tracking/algorithm/nn2dMappingTracker/detector/blob/BlobsDetector.h"

class FishPoseDetector : public IDetector<FishPose>
{
public:

	/**
	* The contructor with parameters.
	* @param: rectification, the rectification object for calculation the real world coordinate.
	* @param: systemProperty, the set of parameters for tracking property.
	*/
	FishPoseDetector(Rectification &rectification, Settings &settings);

	/**
	* The standard destructor.
	*/
	~FishPoseDetector(void);

	/**
	* Gets a fish list from an image
	* @param: image_mat, image contain possible fishs,
	* @return: a fish list.
	*/
	std::vector<FishPose> getPoses(cv::Mat image_mat);

	/**
	* Convert vector of blob poses to fishes poses.
	* @param: blobPoses, vector of blob poses,
	* @return: vector of fish poses.
	*/
	std::vector<FishPose> convertBlobPosesToFishPoses(std::vector<BlobPose> blobPoses);

private:
	BlobsDetector _bd;
};
