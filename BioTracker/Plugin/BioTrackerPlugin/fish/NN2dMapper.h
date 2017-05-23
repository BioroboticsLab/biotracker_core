#pragma once

#include <cv.h>
#include <highgui.h>
#include <chrono>
#include <fish\pose\FishPose.h>
#include <Model\TrackedComponents\TrackedElement.h>
#include <Model\TrackedComponents\TrackedTrajectory.h>
#include "imageProcessor/detector/blob/cvBlob/BlobsDetector.h"

class NN2dMapper
{
public:

	/**
	 * The contructor with parameters.
	 */
	NN2dMapper(TrackedTrajectory *tree);

	~NN2dMapper(void) {};
	
	//TODO HAUKE Document this!
	std::tuple<std::vector<FishPose>, std::vector<float>> getNewPoses(const std::vector<FishPose> &fishPoses, std::vector<BlobPose> blobPoses);
	std::vector<FishPose> convertBlobPosesToFishPoses(std::vector<BlobPose> blobPoses);
	float estimateOrientationRad(int trackid, float *confidence);
	bool correctAngle(int trackid, FishPose &pose);
	
	std::map<int, float> _mapLastConfidentAngle;
	TrackedTrajectory *_tree;

protected:

};

