#pragma once

#include <vector>

#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/model/FishModel.h"
#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/history/History.h"

class Fish
{
public:

	Fish(int id = -1, FishModel model = FishModel(), History history = History());

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

	/**
	 * Gets the model of the fish.
	 * @return: model of the fish.
	 */
	FishModel model() { return _model; }

	/**
	 * Sets the model of the fish.
	 * @return: void.
	 */
	void setModel(FishModel model) { _model = model; }

	/**
	 * Gets the history of the fish.
	 * @return: history of the fish.
	 */
	History history() { return _history; }

	/**
	 * Sets the history of the fish.
	 * @return: void.
	 */
	void setHistory(History history) { _history = history; }

	/**
	 * Updates information of the fish, as a fish model.
	 * @param: fishModel, the fish model.
	 * @return: void.
	 */
	void update(FishModel fishModel);


	/**
	 * Copies all content for the provided fish attributtes to this fish.
	 * @param: fish, the fish to copy.
	 * @return: void.
	 */
	void copyFrom(Fish fish);


	/**
	 * Calculates the orientation of the fish from the saved position in the history.
	 * @return: the orientation of the fish.
	 */
	double getOrientation();

	/**
	 * A static method to get the copied fish provided from id within a given fish list.
	 * @param: fishList, a list of available fish,
	 * @param: id, the id of the fish.
	 * @return: the copied fish with the provided id.
	 */
	static Fish getFishCopyInListById(std::vector<Fish> fishList, int id);

	/**
	 * A static method to get the distance between two fish, calculate the
	 * real metric (centimeter).
	 * @param: fishA, a fish,
	 * @param: fishB, the other fish,
	 * @return: distance between fishA and fishB.
	 */
	static double getDistanceCm(Fish fishA, Fish fishB);

	/**
	 * A static method to get the distance between two fish, calculate the
	 * metric as pixel coordinate.
	 * @param: fishA, a fish,
	 * @param: fishB, the other fish,
	 * @return: distance between fishA and fishB.
	 */
	static double getDistancePx(Fish fishA, Fish fishB);

	/**
	 * A static method to get the distance between two fish, calculate the
	 * metric as pixel coordinate.
	 * @param: fishA, a fish,
	 * @param: point, the other position,
	 * @return: distance between fishA and fishB.
	 */
	static double getDistancePx(Fish fishA, cv::Point point);

	/**
	 * A static method to get the distance between two fish, calculate the
	 * real metric (centimeter).
	 * @param: fishA, a fish,
	 * @param: point2f, the other position,
	 * @return: distance between fishA and fishB.
	 */
	static double getDistanceCm(Fish fishA, cv::Point2f point2f);

private:
	int _id;
	FishModel _model;
	History _history;
};
