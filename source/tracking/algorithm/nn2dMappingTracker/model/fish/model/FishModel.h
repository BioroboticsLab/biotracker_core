#pragma once

#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/pose/FishPose.h"

#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/model/IFishAttributes.h"
#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/model/simple/SimpleFishAttributes.h"

class FishModel
{
public:
	/**
	 * The standard constructor.
	 */
	FishModel(void);

	/**
	 * The constructor with provided parameters.
	 * @param pose, the pose of the fish.
	 * @param ifc, the component object of the fish.
	 */
	FishModel(FishPose pose, IFishAttributes* ifa);

	~FishModel(void);


	/**
	 * Gets the pose of the fish.
	 * @return: pose of the fish.
	 */
	FishPose pose() { return _pose; }

	/**
	 * Sets the pose of the fish.
	 * @return: void.
	 */
	void setPose(FishPose pose) { _pose = pose; }


	void setAttributes(IFishAttributes* ifa) { _ifa = ifa; }

	IFishAttributes* getAttributes() { return _ifa; }

	/**
	 * This dynamic generic Getter-function gets the value of a provided variable name.
	 * the variable are not need to be declared in either the cpp or h. They are just 
	 * declared as dynamic variable, but the names are specified as const std::string
	 * with their namespaces in the cpp file.
	 * @param: attrName, the name of the variable.
	 * @return the value of the variable.
	 */
	template<typename T> T getAttribute(const std::string attrName)
	{
		T t;
		return _ifa->getAttribute(attrName, t);
	}

	/**
	 * This dynamic generic Setter-function sets the value for a provided variable.
	 * This is the countetpart function of the getter function.
	 * @param: attrName, the name of the variable.
	 * @param: attrValue, the value for thisthe variable.
	 * @return void.
	 */
	template<typename T> void setAttribute(const std::string attrName, T attrValue)
	{
		_ifa->setAttribute(attrName,attrValue);
	}

private:
	FishPose _pose;
	IFishAttributes* _ifa;		

};

