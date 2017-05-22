#include "NN2dMapper.h"

#include "helper/CvHelper.h"
#include <limits>
#include <tuple>
#include <utility>

std::tuple<std::vector<FishPose>, std::vector<float>> NN2dMapper::getNewPoses(const std::vector<FishPose> &fishPoses, std::vector<BlobPose> blobPoses) {
	/* The algorithm seems kinda inefficient, as there is many fish*blobs and fish*fish loops.
	 * But as N is expected to be pretty small (<10 for fish, <20 for blobs) this seems feasible.
	*/
	std::vector<FishPose> blobs = convertBlobPosesToFishPoses(blobPoses);
	int sizeF = fishPoses.size(); 
	int sizeB = blobs.size();
	std::vector<std::vector<float>> propMap;
	
	//Create propability matrix
	for (int i = 0; i < sizeF ; i++) {
		std::vector<float> currentFish;
		for (int j = 0; j < sizeB ; j++) {
			currentFish.push_back(FishPose::calculateProbabilityOfIdentity(fishPoses[i], blobs[j]));
		}
		propMap.push_back(currentFish);
	}

	//Walk through all the fish and find the respective best propability
	std::vector<float> bestMatchesProps;
	std::vector<FishPose> bestMatchesPoses;
	for (int i = 0; i < sizeF; i++) {
		float bestMatch = 0;
		FishPose bestMatchPose;
		for (int j = 0; j < propMap[i].size(); j++) {
			if (propMap[i][j] > bestMatch) {
				bestMatch = propMap[i][j];
				bestMatchPose = blobs[j];
			}
		}
		bestMatchesProps.push_back(bestMatch);
		bestMatchesPoses.push_back(bestMatchPose);
	}

	//ok, now we got the most likely pose for each fish at the next time step. 
	//Buuuut, what if two fish are matched to the same pose?
	//Idea is simple: The better match gets the new pose, the other one simply keeps his old pose.
	//Bassically be we use bestMatchesPoses as a NxN matrix and walk through every field.
	//Actually we only need half the matrix, excluding the diagonal line. So that's why i<j.
	//TODO: Maybe use the 2nd best match?
	for (int i = 0; i < bestMatchesPoses.size(); i++) {
		for (int j = 0; j < bestMatchesPoses.size(); j++) {
			if (i < j) {
				if (bestMatchesPoses[i] == bestMatchesPoses[j] && bestMatchesProps[i] <= bestMatchesProps[j]) {
					bestMatchesPoses[i] = fishPoses[i];
					bestMatchesProps[i] = 100; //I guess props are in [0..1], but who cares at this point?
				} 
				else if (bestMatchesPoses[i] == bestMatchesPoses[j] && bestMatchesProps[i] > bestMatchesProps[j]) {
					bestMatchesPoses[j] = fishPoses[j];
					bestMatchesProps[j] = 100; 
				}
			}
		}
	}

	return std::tuple<std::vector<FishPose>, std::vector<float>>(bestMatchesPoses,bestMatchesProps);
}

std::vector<FishPose> NN2dMapper::convertBlobPosesToFishPoses(std::vector<BlobPose> blobPoses)
{
	std::vector<FishPose> fishPoses;
	fishPoses.reserve(blobPoses.size());

	for (BlobPose & blobPose : blobPoses)
	{
		fishPoses.push_back(
			FishPose(
				blobPose.posCm(),
				blobPose.posPx(),
				CvHelper::degToRad(blobPose.angleDegree()),
				blobPose.angleDegree(),
				blobPose.width(),
				blobPose.height()
			)
		);
	}

	return fishPoses;
}
