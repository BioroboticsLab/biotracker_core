#include "IFishAttributes.h"

IFishAttributes::IFishAttributes(
	std::map<const std::string, bool> boolAttributes,
	std::map<const std::string, int>  intAttributes,
	std::map<const std::string, float>  floatAttributes,
	std::map<const std::string, double>  doubleAttributes) :
	_boolAttributes(boolAttributes),
	_intAttributes(intAttributes),
	_floatAttributes(floatAttributes),
	_doubleAttributes(doubleAttributes)
{
}

IFishAttributes::~IFishAttributes(void)
{
}

template<> bool IFishAttributes::getAttribute(const std::string attrName, bool &value)
{
	for( std::map<std::string, bool>::iterator ii=_boolAttributes.begin(); ii!=_boolAttributes.end(); ++ii)
	{
		if((*ii).first.compare(attrName) == 0)
		{
			value = (*ii).second;
			break;
		}

	}

	return value;
}

template<> int IFishAttributes::getAttribute(const std::string attrName, int &value)
{	
	for( std::map<std::string, int>::iterator ii=_intAttributes.begin(); ii!=_intAttributes.end(); ++ii)
	{
		if((*ii).first.compare(attrName) == 0)
		{
			value = (*ii).second;
			break;
		}

	}

	return value;
}

template<> float IFishAttributes::getAttribute(const std::string attrName, float &value)
{
	for( std::map<std::string, float>::iterator ii=_floatAttributes.begin(); ii!=_floatAttributes.end(); ++ii)
	{
		if((*ii).first.compare(attrName) == 0)
		{
			value = (*ii).second;
			break;
		}

	}

	return value;
}

template<> double IFishAttributes::getAttribute(const std::string attrName, double &value)
{
	for( std::map<std::string, double>::iterator ii=_doubleAttributes.begin(); ii!=_doubleAttributes.end(); ++ii)
	{
		if((*ii).first.compare(attrName) == 0)
		{
			value = (*ii).second;
			break;
		}

	}

	return value;
}

template<> void IFishAttributes::setAttribute(const std::string attrName, bool attrValue)
{
	_boolAttributes[attrName] = attrValue;	
}

template<> void IFishAttributes::setAttribute(const std::string attrName, int attrValue)
{
	_intAttributes[attrName] = attrValue;	
}

template<> void IFishAttributes::setAttribute(const std::string attrName, float attrValue)
{
	_floatAttributes[attrName] = attrValue;	
}

template<> void IFishAttributes::setAttribute(const std::string attrName, double attrValue)
{
	_doubleAttributes[attrName] = attrValue;	
}