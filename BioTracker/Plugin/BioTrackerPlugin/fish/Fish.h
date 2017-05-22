#pragma once

#include "property/IParam.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

#include "fish/history/History.h"
#include "fish/pose/FishPose.h"

namespace FISHATTRIBUTES
{
	const std::string ROBOT  = "robot";
	const std::string ACTIVE = "active";
	const std::string MARKED = "marked";
	const std::string HISTORYLENGTH = "historyLength";
}

class Fish : public IParam
{
public:

	Fish();
	Fish(const Fish& fish);
	Fish(const Fish& fish, const FishPose &currentPose, const History &history);
	Fish(int id, bool robot, bool active, bool marked, const FishPose &currentPose, const History &history);

	~Fish(void);

	/**
	 * Gets the id of the fish.
	 * @return: id of the fish.
	 */
	int id() const { return _id; }

	/**
	 * Sets the id of the fish.
	 * @param: id, the id to set.
	 * @return: void.
	 */
	void setId(int id) { _id = id; }

	bool isRobot() const { return _robot; }
	bool isActive() const { return _active; }
	bool isMarked() const { return _marked; }

	FishPose currentPose() const { return _currentPose; }

	/**
	* Extrapolates the current trajectory and estimates the next pose.
	* @return: estimation of next pose or nullptr
	*/
	std::shared_ptr<FishPose> estimateNextPose() const;

	/**
	* Returns a pose that can be used for mapping.
	* That is either the estimated next pose or the current pose if the estimation failed.
	* @return: estimated pose or current pose
	*/
	FishPose getPoseForMapping() const;

	/**
	 * Gets the history of the fish.
	 * @return: history of the fish.
	 */
	History history() const;

	/**
	 * Sets the history of the fish.
	 * @return: void.
	 */
	void setHistory(History history);

	/**
	 * Update the histoy length of the fish object.
	 * @param: length, the history length.
	 * @return: void.
	 */
	void updateHistoryLength(int length);

	/**
	 * Updates information of the fish, as a fish model.
	 * @param: fishModel, the fish model.
	 * @return: void.
	 */
	void update(FishPose fishPose);


	/**
	 * Copies all content for the provided fish attributtes to this fish.
	 * @param: fish, the fish to copy.
	 * @return: void.
	 */
	void copyFrom(Fish& fish);

	/**
	* @return: the last confident orientation of the fish in radians.
	*/
	float getLastConfidentOrientationRad() const { return _lastConfidentOrientationRad; };

	/**
	* Sets a orientation that is thought to be highly accurate.
	*/
	void setLastConfidentOrientationRad(float orientation) { _lastConfidentOrientationRad = orientation; };

	void setInt(std::string spec_param, int value);
	void setBool(std::string spec_param, bool value);
	void setFloat(std::string spec_param, float value);
	void setDouble(std::string spec_param, double value);

	/**
	 * A static method to find a fish with an id within a given fish list.
	 * @param: fishList, a list of available fish,
	 * @param: id, the id of the fish.
	 * @return: iterator pointing at the fish with the right id or iterator pointing at fishList.end()
	 */
	static std::vector<Fish>::iterator getFishPositionInListById(std::vector<Fish>& fishList, int id);

	/**
	 * A static method to get the distance between two fish, calculate the
	 * real metric (centimeter).
	 * @param: fishA, a fish,
	 * @param: fishB, the other fish,
	 * @return: distance between fishA and fishB.
	 */
	static double getDistanceCm(Fish& fishA, Fish& fishB);

	/**
	* A static method to get the distance between two fish, calculate the
	* real metric (centimeter) and take the orientation into account.
	* @param: fishA, a fish,
	* @param: fishB, the other fish,
	* @return: distance between fishA and fishB.
	*/
	static double getDistanceCmDegree(Fish& fishA, Fish& fishB);

	/**
	 * A static method to get the distance between two fish, calculate the
	 * metric as pixel coordinate.
	 * @param: fishA, a fish,
	 * @param: fishB, the other fish,
	 * @return: distance between fishA and fishB.
	 */
	static double getDistancePx(Fish& fishA, Fish& fishB);

	/**
	 * A static method to get the distance between two fish, calculate the
	 * metric as pixel coordinate.
	 * @param: fishA, a fish,
	 * @param: point, the other position,
	 * @return: distance between fishA and fishB.
	 */
	static double getDistancePx(Fish& fishA, cv::Point& point);

	/**
	 * A static method to get the distance between two fish, calculate the
	 * real metric (centimeter).
	 * @param: fishA, a fish,
	 * @param: point2f, the other position,
	 * @return: distance between fishA and fishB.
	 */
	static double getDistanceCm(Fish& fishA, cv::Point2f& point2f);

	static bool compareOnId(const Fish& fL, const Fish& fR) { return fL.id() < fR.id(); }

private:
	int _id;	
	bool _robot;
	bool _active;
	bool _marked;

	FishPose _currentPose;
	History _history;	

	void setRobot (bool robot)  { _robot = robot;   }
	void setActive(bool active) { _active = active; }
	void setMarked(bool marked) { _marked = marked; }

	/* The last orientation that was measured with a high amount of confidence.
	This can be used to estimate or smooth the current direction. */
	float _lastConfidentOrientationRad;
};

