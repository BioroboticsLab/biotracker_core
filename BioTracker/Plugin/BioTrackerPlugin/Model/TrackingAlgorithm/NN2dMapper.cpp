#include "NN2dMapper.h"

#include "helper/CvHelper.h"
#include <limits>
#include <tuple>
#include <utility>

float dif(float a, float b) {
	return fmod((std::abs(b - a)), CV_PI);
}

NN2dMapper::NN2dMapper(TrackedTrajectory *tree) {
	_tree = tree;

	//Looks kinda complicated but is a rather simple thing:
	//For every true trajectory below the tree's root (which are in fact, fish trajectories in out case)
	//we want to arr a last confident angle to the map.
	for (int i = 0; i < _tree->numberOfChildrean(); i++)
	{
		TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(_tree->getChild(i));
		if (t)
			_mapLastConfidentAngle.insert(std::pair<int, float>(i, std::numeric_limits<float>::quiet_NaN()));
	}
}

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

	for (int i = 0; i < bestMatchesPoses.size(); i++) {
		if (!(bestMatchesPoses[i] == fishPoses[i])) {
			bool angleConfident = correctAngle(i, bestMatchesPoses[i]);
			if (angleConfident) _mapLastConfidentAngle.at(i) = bestMatchesPoses[i].orientation_rad();

			//NOTE: I was hoping to do something meaningful with a much less complicated approach,
			//		but I'm not convinced by these results at all
			/*cv::Point2f p1		= bestMatchesPoses[i].position_cm();
			cv::Point2f p2		= fishPoses[i].position_cm();
			cv::Point2f p3		= p2 - p1;
			float a				= cvFastArctan(p3.x, p3.y);
			float byMovementDeg	= fmod((a + 90.f), 360.f);
			float byMovementRad = (byMovementDeg / 360.f) * (CV_PI * 2.f);
			float byBlobRad		= bestMatchesPoses[i].orientation_rad();
			float byprevRad		= fishPoses[i].orientation_rad();

			if (abs(byBlobRad- byMovementRad) > CV_PI / 2.f)
				byBlobRad = byMovementRad;
			else if (abs(byBlobRad-byprevRad) > CV_PI / 2.f)
				byBlobRad = byprevRad;

			if (byBlobRad - byprevRad > 0.1)
				byBlobRad = byprevRad - 0.1;
			else if (byBlobRad - byprevRad < -0.1)
				byBlobRad = byprevRad + 0.1;

			bestMatchesPoses[i].set_orientation_rad(byBlobRad);*/
		}
	}

	return std::tuple<std::vector<FishPose>, std::vector<float>>(bestMatchesPoses,bestMatchesProps);
}

bool NN2dMapper::correctAngle(int trackid, FishPose &pose)
{
	// the current angle is a decent estimation of the direction; however, it might point into the wrong hemisphere
	const float poseOrientation = pose.orientation_rad();

	// start with the pose orientation for our estimate
	float proposedAngle = poseOrientation;

	// we have more historical data to correct the new angle to at least be more plausible
	float confidence = 0.0f;
	const float historyAngle = estimateOrientationRad(trackid, &confidence);
	//const float lastConfidentAngle = fish.getLastConfidentOrientationRad();
	float lastConfidentAngle = _mapLastConfidentAngle.at(trackid); //TODO Hauke check if this is an ok thing to do...

	// the current history orientation has a stronger meaning and is preferred
	const float comparisonOrientation = std::isnan(historyAngle) ? lastConfidentAngle : historyAngle;
	// can't correct the angle? 
	if (std::isnan(comparisonOrientation)) return false;

	// panic mode - what if nothing was measured?
	if (std::isnan(poseOrientation))
	{
		pose.set_orientation_rad(comparisonOrientation);
		pose.set_orientation_deg(comparisonOrientation * 180.0f / CV_PI);
		return false;
	}

	const float angleDifference = CvHelper::angleDifference(proposedAngle, comparisonOrientation);

	// if the angles do not lie on the same hemisphere, mirror the proposed angle
	if (!std::isnan(angleDifference) && std::abs(angleDifference) > 0.5f * CV_PI)
	{
		proposedAngle += CV_PI;
	}

	// the angle is corrected into the correct hemisphere now;
	// now smooth the angle to reduce the impact of outliers or directly remove a zero-measurement.

	if (std::isnan(lastConfidentAngle)) // nothing to smooth? Then simply assume the movement-angle to be a good first estimate
		proposedAngle = historyAngle;
	else
	{
		// smooth the change in the angle iff the new angle deviates too much from the last one
		const float deviationFromLast = CvHelper::angleDifference(lastConfidentAngle, proposedAngle);
		assert(!std::isnan(deviationFromLast));
		
		if (std::abs(deviationFromLast) > 0.2f * CV_PI)
		{
			if (poseOrientation == 0.0f) // deviation AND zero-angle? Most likely not a decent estimation.
				proposedAngle = lastConfidentAngle;
			else // smooth outliers by a fixed margin
				proposedAngle = lastConfidentAngle - 0.1f * deviationFromLast;
		}
	}
	// angle should be between 0° and 360°
	if (proposedAngle > 2.0f * CV_PI) proposedAngle -= 2.0f * CV_PI;
	else if (proposedAngle < 0.0f)    proposedAngle += 2.0f * CV_PI;
	assert(!std::isnan(proposedAngle));

	pose.set_orientation_rad(proposedAngle);
	pose.set_orientation_deg(proposedAngle * 180.0f / CV_PI);

	// did we have ANY confident correction?
	if (!std::isnan(lastConfidentAngle)) // if we simply adjusted the last position, assume to be confident
		return true;
	// otherwise, we need to intialize the confident angle.
	// do that when we really are "confident" for the first time..
	const float differenceToHistoryAngle = std::abs(CvHelper::angleDifference(proposedAngle, historyAngle));
	assert(!std::isnan(differenceToHistoryAngle));
	if (differenceToHistoryAngle < 0.25f * CV_PI)
		return true;
	// neither updating nor a good initialization?
	return false;
}

float NN2dMapper::estimateOrientationRad(int trackid, float *confidence)
{
	//Get corresponding trajectory
	TrackedTrajectory* t = (TrackedTrajectory*)_tree->getChild(trackid);

	// can't give estimate if not enough poses available
	if (t->numberOfChildrean() < 3) return std::numeric_limits<float>::quiet_NaN();

	//std::deque<FishPose>::const_reverse_iterator iter = _histComponents.rbegin();
	int start = std::max(t->numberOfChildrean()-20, 0);
	TrackedElement* e = (TrackedElement*)t->getChild(start);
	cv::Point2f nextPoint = e->getFishPose().position_cm();
	cv::Point2f positionDerivative(0.0f, 0.0f);

	// weights the last poses with falloff^k * pose[end - k] until falloff^k < falloffMargin
	int posesUsed = 0;
	float currentWeight = 1.0f;
	float weightSum = 0.0f;
	const float falloff = 0.9f;
	const float falloffMargin = 0.4f;

	for (int i=start+1; i<t->numberOfChildrean(); i++)
	{
		TrackedElement* ecur = (TrackedElement*)t->getChild(i);
		cv::Point2f currentPoint = ecur->getFishPose().position_cm();
		const cv::Point2f oneStepDerivative = nextPoint - currentPoint;

		positionDerivative += currentWeight * oneStepDerivative;
		weightSum += currentWeight;

		currentWeight *= falloff;
		if (currentWeight < falloffMargin) break;

		nextPoint = currentPoint;
		++posesUsed;
	}
	// calculate average (weighted) movement of the fish
	if (weightSum != 0.0f)
	{
		positionDerivative.x /= weightSum;
		positionDerivative.y /= weightSum;
	}
	// use the euclidian distance in cm
	const float distance = std::sqrt(std::pow(positionDerivative.x, 2.0f) + std::pow(positionDerivative.y, 2.0f));
	// Calculate cm/s.
	const float distanceNormalized = 1000.0f * distance / 33.3; // TODO Hauke static_cast<float>(FishTrackerThread::instance()->getRealTimePerFrameMs())
	const float confidenceDistanceMinCm = 2.0f;
	const float confidenceDistanceMaxCm = 6.0f;
	// if we have either nearly no data or are very unsure (left movement offsets right movement f.e.), just return nothing
	if (distanceNormalized < confidenceDistanceMinCm)
		return std::numeric_limits<float>::quiet_NaN();
	*confidence = std::min(distanceNormalized / confidenceDistanceMaxCm, 1.0f);

	// negative y coordinate to offset open cv coordinate system
	return std::atan2(-positionDerivative.y, positionDerivative.x);
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
