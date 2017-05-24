#pragma once

#include "Model/TrackedComponents/pose/FishPose.h"
#include "Model/TrackingAlgorithm/property/SystemProperty.h"
#include "Model/TrackingAlgorithm/property/Rectification.h"
#include "Model/TrackingAlgorithm/imageProcessor/detector/blob/BlobPose.h"

template <typename T>
class IDetector
{
public:

	/**
	* The contructor with parameters.
	* @param: rectification, the rectification object for calculation the real world coordinate.
	* @param: systemProperty, the set of parameters for tracking property.
	*/
	IDetector(void) {}

	/**
	* The standard abtract destructor.
	*/
	~IDetector(void) {}

	/**
	* Interface function to get the fish poses from a binarzied image.
	* @param: binarized_image_mat, binarized image.
	* @return: a list of fish poses.
	*/	
	virtual std::vector<T> getPoses(cv::Mat& binImage, cv::Mat& oriImage) = 0;


	/**
	* Convert vector of blob poses to fishes poses.
	* @param: blobPoses, vector of blob poses,
	* @return: vector of fish poses.
	*/
	static std::vector<FishPose> convertBlobPosesToFishPoses(std::vector<BlobPose> blobPoses);

	/**
	 * Sets specified parameter with a double value.
	 * @param: spec_param, the specified parameter to set,
	 * @param: value, the value double to set,
	 * @return: void.
	 */
	virtual void setDouble(std::string spec_param, double value) = 0;

	/**
	 * Gets the specified parameter.
	 * @param: spec_param, the specified parameter to set,
	 * @return: the double value of the specifed paramter.
	 */
	//virtual void getDouble(std::string spec_param) = 0;

private:
	virtual std::vector<T> findBlobs(const cv::Mat& binImage, const cv::Mat& oriImage) = 0;

protected:

};
