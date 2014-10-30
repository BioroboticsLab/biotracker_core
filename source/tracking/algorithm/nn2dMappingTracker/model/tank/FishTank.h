#pragma once

#include <vector>
#include <fstream>

#include "core/fish/Fish.h"
#include "property/SystemProperty.h"

class FishTank
{
public:

	/**
	 * The contructor with parameters.
	 * @param: fishAmount, optional parameter for the amount of fishes
	 * which will be created in the fish tank.
	 */
	FishTank(SystemProperty &systemProperty);

	~FishTank(void);

	/**
	 * Initilizes the fish tank with a provided number of fish.
	 * @param: fishNumber, the amount of fish.
	 * @return: void.
	 */
	void initFishes(int fishNumber = 0);

	/**
	 * Gets all fish of the fish tank.
	 * @return: afish list.
	 */
	std::vector<Fish> fishes();

	/**
	 * Updates the fishes within the fish tank.
	 * @param: the new fish list to set into the the fish tank.
	 * @return: void.
	 */
	void updateFishes(std::vector<Fish> fishes);

	/**
	 * Adds a new fish to the fish tank.
	 * @return: void;
	 */
	void addFish(Fish fish);

	/**
	 * Removes a fish from the fish list.
	 * @return: true, if fish was removed from fish list, false if removing action failed.
	 * Failed, when fish is not found in the fish list.
	 */
	bool removeFish(Fish fish);

	/**
	 * Gets the current center centimeter value, the mean of the all fish positions, of the fish shoal.
	 * @return: the mean position of al fish.
	 */
	cv::Point2f currentSwarmCenterCm();

	/**
	 * Gets the current center as pixel value, the mean of the all fish positions, of the fish shoal.
	 * @return: the mean position of al fish.
	 */
	cv::Point currentSwarmCenterPx();

	/**
	 * Swaps the id of a selected fish with the fish which has the provided ip.
	 * @param: newId, the new id for the selected fish at the specified position.
	 * @param: p, the clicked position.
	 * @return: void.
	 */
	void swapFishId(int newId, cv::Point p);

	/**
	 * Renew the id of the fish with the provided old id.
	 * @param: oldId, the old id of the fish.
	 * @param: newId, the new id to set.
	 * @return: void.
	 */
	void setFishOfIdWithId(int oldId, int newId);

	/**
	 * Gets the nearest fish.
	 * @param: point, current location.
	 * @return: the nearest availeable fish.
	 */
	int getNearestFishIdToLocation(cv::Point point);

	/**
	 * Gets the fish by id.
	 * @param: id, the id of the fish.
	 * @return: the fish with the provided id.
	 */
	Fish getFishById(int id);

	/**
	 * Marks the fish as the robofish.
	 *  @param: position, current location of the robofish.
	 * @return: void.
	 */
	void toggleRobofish(cv::Point position);

	const std::vector<cv::Point2f>& polygon_cm() const { return _polygon_cm; }
	void setPolygon_cm(const std::vector<cv::Point2f>& polygon_cm) { _polygon_cm = polygon_cm; }

protected:
	std::vector<Fish> _fishes;
	std::vector<cv::Point2f> _polygon_cm;
	SystemProperty &_systemProperty;
	QMutex _fishMutex;
};
