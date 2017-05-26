#pragma once

#include <cv.h>
#include <highgui.h>
#include <chrono>
#include <fish/pose/FishPose.h>
#include <Model/TrackedComponents/TrackedElement.h>
#include "imageProcessor/detector/blob/cvBlob/BlobsDetector.h"

class NN2dMapper
{
public:

	/**
	 * The contructor with parameters.
	 */
	NN2dMapper(void) {}

	~NN2dMapper(void) {}
	std::tuple<std::vector<FishPose>, std::vector<float>> getNewPoses(const std::vector<FishPose> &fishPoses, std::vector<BlobPose> blobPoses);
	std::vector<FishPose> convertBlobPosesToFishPoses(std::vector<BlobPose> blobPoses);

	/**
	 * Function to map each fish to a nearest fish poses.
	 * @param: fishes, a fish list to map,
	 * @param: fishPoses, a fish pose list need for mapping.
	 * @return: a mapped fish list.
	 */
        std::vector<TrackedElement> map(std::vector<TrackedElement> fishes, std::vector<FishPose> fishPoses, const cv::Mat& frame = cv::Mat(), const std::deque<cv::Mat>& oldFrames = std::deque<cv::Mat>());

protected:

	/**
	 * This method remaps the fish list.
	 * @param: problemFishIds, the list of fish ids, which might be mapped wrong.
	 * @param: mappedFishList, a fish list, which was mapped and merged.
	 * @param: oldFishList, the old fish list.
	 * @param: curImg, the current image.
	 * @param: oldImgs, a list of old images.
	 * @return: the remapped fish list.
	 */
        std::vector<TrackedElement> remap(std::vector<int> problemFishIds, std::vector<TrackedElement> mappedFishList, std::vector<TrackedElement> oldFishList, const cv::Mat& frame = cv::Mat(), const std::deque<cv::Mat>& oldFrames = std::deque<cv::Mat>());

	/**
	 * Verify the mapping, whether a certain cretia accept the right mapping.
	 * @param: newFishList, a fish list, which was mapped and merged.
	 * @param: oldFishList, the old fish list.
	 * @return: a list of wrong mapping fish indices. If list is empty mapping was correct.
	 */
	std::vector<int> validate(std::vector<TrackedElement> newFishList, std::vector<TrackedElement> oldFishList);

	/**
	 * Finds the nearest pose for the according fish.
	 * @param: fishPose, a fish pose.
	 * @param: fishPoses, all possible available poses.
	 * @return: the nearest pose.
	 */
	std::tuple<int, float> getNearestIndexFromFishPoses(const FishPose &fishPose, const std::vector<FishPose> &fishPoses);

	/**
	* Finds the nearest pose for the according fish.
	* @param: fish, a fish object.
	* @param: fishPoses, all possible available poses.
	* @return: the nearest pose.
	*/
        int getNearestIndexFromFishPoses(TrackedElement fish, std::vector<FishPose> fishPoses);

	/**
	 * This function is a recursive function, which tries to find the really nearest fish
	 * to the provided fish poses list. Which means: recursively walking through all possible
	 * shortest distance for each fish, and determine whether it is its real nearest neighbour.
	 * @param: fishIndex, the index of the fish in the the fish list.
	 * @param: fishes, the fish list as a reference.
	 * @param: fishPoses, the fish poses list as a reference.
	 * @return: the fish with its updated posistion 
	 *(distance of the old position and the new in the provided fish poses list).
	 */
	TrackedElement mergePoseToFish(int fishIndex, std::vector<TrackedElement> &fishes, std::vector<FishPose> &fishPoses);

	/**
	 * Corrent the angle of the fish.
	 * @param: fish, the old information of the fish.
	 * @param: newFishpose, the new fish pose.
	 * @return: whether the angle was corrected.
	 */
	bool correctAngle(TrackedElement &fish, FishPose &newFishpose);
};

