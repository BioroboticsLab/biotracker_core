#include "IParam.h"

#include "helper/StringHelper.h"

template<> bool IParam::get(const std::string attrName, bool &value)
{
	throw std::runtime_error("Function not implmented yet"); 

	return value;
}

template<> int IParam::get(const std::string attrName, int &value)
{	
	throw std::runtime_error("Function not implmented yet"); 

	return value;
}

template<> float IParam::get(const std::string attrName, float &value)
{
	throw std::runtime_error("Function not implmented yet"); 

	return value;
}

template<> double IParam::get(const std::string attrName, double &value)
{
	throw std::runtime_error("Function not implmented yet"); 

	return value;
}

template<> void IParam::set(const std::string attrName, bool attrValue)
{
	throw std::runtime_error("Function not implmented yet"); 
}

template<> void IParam::set(const std::string attrName, int attrValue)
{
	throw std::runtime_error("Function not implmented yet");
}

template<> void IParam::set(const std::string attrName, float attrValue)
{
	throw std::runtime_error("Function not implmented yet");
}

template<> void IParam::set(const std::string attrName, double attrValue)
{
	throw std::runtime_error("Function not implmented yet");
}