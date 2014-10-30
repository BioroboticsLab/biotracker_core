#include "SimpleFishAttributes.h"

SimpleFishAttributes::SimpleFishAttributes()
{
	initVariables();	
}

SimpleFishAttributes::SimpleFishAttributes(
	std::map<const std::string, bool> boolAttributes,
	std::map<const std::string, int>  intAttributes,
	std::map<const std::string, float>  floatAttributes,
	std::map<const std::string, double>  doubleAttributes) :
	IFishAttributes(boolAttributes,intAttributes,floatAttributes,doubleAttributes)
{
}

SimpleFishAttributes::~SimpleFishAttributes(void)
{
}


void SimpleFishAttributes::initVariables()
{
	setAttribute<bool>(SimpleFishAttributes_VARIABLES::BOOL::ROBOT,false);
	setAttribute<bool>(SimpleFishAttributes_VARIABLES::BOOL::ACTIV,false);
}
