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
	int cid = 0;
	for (int i = 0; i < _tree->size(); i++)
	{
		TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(_tree->getChild(i));
		if (t && t->getValid()){
			_mapLastConfidentAngle.insert(std::pair<int, float>(cid, std::numeric_limits<float>::quiet_NaN()));
			cid++;
		}
	}
}

// Functor to compare by the Mth element, as per https://stackoverflow.com/questions/23030267/custom-sorting-a-vector-of-tuples
struct TupleCompare
{
	bool operator()(std::tuple<float, FishPose> a, std::tuple<float, FishPose> b) const {
		return std::get<0>(a) > std::get<0>(b);
	}

};

FishPose getFishpose(TrackedTrajectory* traj, uint frameid, uint id) {
    IModelTrackedComponent* comp = traj->getValidChild(id);
    TrackedTrajectory* ct = dynamic_cast<TrackedTrajectory*>(comp);
    if (ct) {
        TrackedElement *el = dynamic_cast<TrackedElement*>(ct->getChild(frameid-1));
        if (el)
            return el->getFishPose();
    }
    return FishPose();
}

std::tuple<std::vector<FishPose>, std::vector<float>> NN2dMapper::getNewPoses(TrackedTrajectory* traj, uint frameid, std::vector<BlobPose> blobPoses) {
	/* The algorithm seems kinda inefficient, as there is many fish*blobs and fish*fish loops.
	 * But as N is expected to be pretty small (<10 for fish, <20 for blobs) this seems feasible.
	*/
	std::vector<FishPose> blobs = convertBlobPosesToFishPoses(blobPoses);

	int sizeF = traj->validCount();
	int sizeB = blobs.size();
	std::vector<std::vector<std::tuple<float, FishPose>>> propMap;
	
	//Create propability matrix and sort it
	for (int i = 0; i < sizeF ; i++) {
		std::vector<std::tuple<float, FishPose>> currentFish;
        FishPose cpose = getFishpose(traj, frameid, i);
		for (int j = 0; j < sizeB ; j++) {
            currentFish.push_back(std::tuple<float, FishPose>(FishPose::calculateProbabilityOfIdentity(cpose, blobs[j]), blobs[j]));
		}
		std::sort(begin(currentFish), end(currentFish), TupleCompare());
		propMap.push_back(currentFish);
	}

	//I'm sorry for the goto/inefficient loop. But usually we will not have more than a 
	//hand full (<6) of blobs to walk through, so it's not worth the optimizing.
	retry:
	for (int i = 0; i < propMap.size(); i++) {
		for (int j = 0; j < propMap.size(); j++) {
			//Is this 
			if (!propMap[i].empty() && !propMap[j].empty() && i!=j) {
				//Reads: If same blob, but i has higher props than j
				bool samePose = std::get<1>(propMap[i][0]) == std::get<1>(propMap[j][0]);
				bool propLess = std::get<0>(propMap[i][0]) <= std::get<0>(propMap[j][0]);
				if (samePose && propLess) {
					//...then remove the 0'th element from j and try again.
					propMap[i].erase(propMap[i].begin());
					goto retry;
				}
			}
		}
	}


	std::vector<float> bestMatchesProps;
	std::vector<FishPose> bestMatchesPoses;
	for (int i = 0; i < propMap.size(); i++) {
		if (propMap[i].size() > 0) {
			bestMatchesProps.push_back(std::get<0>(propMap[i][0]));
			bestMatchesPoses.push_back(std::get<1>(propMap[i][0]));
		}
		else {
			bestMatchesPoses.push_back(getFishpose(traj, frameid, i));
			bestMatchesProps.push_back(100);
		}
	}
	
	for (int i = 0; i < bestMatchesPoses.size(); i++) {

        //Look at what the fish did in the last 10 frames
        double historyDir;
        {
            double lookBack = 10;
            cv::Point2f p = getFishpose(traj, frameid - lookBack, i).position_cm();
            cv::Point2f pnow = bestMatchesPoses[i].position_cm();
            while (CvHelper::getDistance(p, pnow) < 0.1 && lookBack < 100) {
                lookBack += 10;
                cv::Point2f p = getFishpose(traj, frameid - lookBack, i).position_cm();
                cv::Point2f pnow = bestMatchesPoses[i].position_cm();
            }
            double dir = CvHelper::getAngleToTarget(p, pnow);
            //correct some weird angle definition
            historyDir = dir + CV_PI / 2;
        }

        //The blob detection will come up with an ellipse orientation, where front and back are ambigious.
        //So check the history for movement direction. Use the history as an indicator where front and back are.
        //This might be unstable iff the fish didn't move at all.
        double dif = CvHelper::angleDifference(bestMatchesPoses[i].orientation_rad(), historyDir);
        if (std::abs(dif) > CV_PI / 2) {
            double dir = bestMatchesPoses[i].orientation_rad() + CV_PI;
            while (dir >  2 * CV_PI) dir -= 2 * CV_PI;
            while (dir < -2 * CV_PI) dir += 2 * CV_PI;
            bestMatchesPoses[i].set_orientation_rad(dir);
            bestMatchesPoses[i].set_orientation_deg(dir * 180.0f / CV_PI);
        }

	
		//TODO move this to algorithm because this does not really take user manipulation into consideration
		//TODO include difference between estimated and current
		//simple smoothing out big angle jumps
		float lastConfidentAngle = _mapLastConfidentAngle.at(i); //this is mot really working if we jump some frmae back/fwd
		double dir = bestMatchesPoses[i].orientation_rad();
		if(!std::isnan(lastConfidentAngle)){
			const float deviationFromLast = CvHelper::angleDifference(lastConfidentAngle, dir);
			if (std::abs(deviationFromLast) > CV_PI * 0.2 ){ // 36°
				dir += deviationFromLast * 0.5; //reduce big jumps
			}
			else{
				dir += deviationFromLast * 0.1;//smooth out small changes
			}
			 while (dir >  2 * CV_PI) dir -= 2 * CV_PI;
            while (dir < -2 * CV_PI) dir += 2 * CV_PI;
			bestMatchesPoses[i].set_orientation_rad(dir);
            bestMatchesPoses[i].set_orientation_deg(dir * 180.0f / CV_PI);
		}
		_mapLastConfidentAngle[i] = dir;
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
	// angle should be between 0� and 360�
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

TrackedTrajectory* getChildOfType(TrackedTrajectory* tree, int tid) {
	int cid = 0;
	for (int i = 0; i < tree->size(); i++) {
		TrackedTrajectory* t = dynamic_cast<TrackedTrajectory*>(tree->getChild(i));
		if (t && cid==tid && t->getValid()) {
			return t;
		}else if (t && t->getValid())
			cid++;
	}
	return 0;
}

float NN2dMapper::estimateOrientationRad(int trackid, float *confidence)
{
	//Get corresponding trajectory
	TrackedTrajectory* t = getChildOfType((TrackedTrajectory*)_tree, trackid);
    //return 0;

	// can't give estimate if not enough poses available
	if (t->size() < 3) return std::numeric_limits<float>::quiet_NaN();

	//std::deque<FishPose>::const_reverse_iterator iter = _histComponents.rbegin();
	int start = std::max(t->size()-20, 0);
	TrackedElement* e = (TrackedElement*)t->getChild(start);
	if (!e)
		return std::numeric_limits<float>::quiet_NaN();
	cv::Point2f nextPoint = e->getFishPose().position_cm();
	cv::Point2f positionDerivative(0.0f, 0.0f);

	// weights the last poses with falloff^k * pose[end - k] until falloff^k < falloffMargin
	int posesUsed = 0;
	float currentWeight = 1.0f;
	float weightSum = 0.0f;
	const float falloff = 0.9f;
	const float falloffMargin = 0.4f;

	for (int i=start+1; i<t->size(); i++)
	{
		TrackedElement* ecur = (TrackedElement*)t->getChild(i);
		if (!ecur)
			return std::numeric_limits<float>::quiet_NaN();
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
