#pragma once

#include "source/tracking/algorithm/nn2dMappingTracker/model/fish/model/IFishAttributes.h"

namespace SimpleFishAttributes_VARIABLES
{
	namespace BOOL
	{
		const std::string ROBOT = "robot";
		const std::string ACTIV = "active";
	}

	namespace INT
	{
	}

	namespace FLOAT
	{
	}

	namespace DOUBLE
	{
	}
}

class SimpleFishAttributes : public IFishAttributes
{
public:

	/**
	 * The standard constructor.
	 */
	SimpleFishAttributes(void);

	SimpleFishAttributes::SimpleFishAttributes(
		std::map<const std::string, bool> boolAttributes,
		std::map<const std::string, int>  intAttributes,
		std::map<const std::string, float>  floatAttributes,
		std::map<const std::string, double>  doubleAttributes
	);

	/**
	 * The standard destructor.
	 */
	virtual ~SimpleFishAttributes(void);

	void initVariables();
};