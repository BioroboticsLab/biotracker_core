#pragma once

#include <deque>

#include "fish/pose/FishPose.h"

class History
{
public:
	History();
	History(const History& history);
	History(int histMaxSize);
	~History(void);

	/**
	 * Adds a fish pose entry two the history list.
	 * @param: fc, a fishpose component.
	 * return: void;
	 */
	void add(FishPose fishState);

	/**
	 * Removes the last entry of the history list.
	 * return: void;
	 */
	void removeLast();

	/**
	* Estimates the orientation of the fish based on the last poses if available.
	* This can be used to adjust the blob-orientation (which might point into the wrong hemisphere).
	* @return: orientation estimate.
	*/
	float estimateOrientationRad(float *confidence) const;

	/**
	* Calculates the current speed of a fish. The speed is the average of the derivation of the fish's position over X frames (smoothingWindow).
	* This can estimate the speed either in pixels or in cm, depending on the parameter. Defaults to cm.
	* @return: the speed of the fish.
	*/
	float getCurrentSpeed(int smoothingWindow=3, cv::Point2f (FishPose::*positionFunction)() const = &FishPose::position_cm) const;

	void updateLength(int newLength);
	int getHistoryLength() const;
	/*
	* @return: the number of poses currently in the history
	*/
	int getPoseCount() const;

	std::deque<FishPose> poses();

private:
	int _histMaxSize;
	int _nextInsertedPosition;
	std::deque<FishPose> _histComponents;
};

