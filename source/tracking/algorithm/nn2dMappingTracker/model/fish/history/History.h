#pragma once

#include <deque>
#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/model/FishModel.h"
#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/pose/FishPose.h"

class History
{
public:
	History(int histMaxSize = 0);
	~History(void);

	/**
	 * Adds a fish component entry two the history list.
	 * @param: fc, a fish component.
	 * return: void;
	 */
	void add(FishModel fc);

	/**
	 * Removes the last entry of the history list.
	 * return: void;
	 */
	void removeLast();

	/**
	 * Calculates the orientation of the fish from the saved position in the history.
	 * @return: the orientation of the fish.
	 */
	double orientation();

	std::vector<FishPose> poses();

private:
	int _histMaxSize;
	std::deque<FishModel> _histComponents;
};

