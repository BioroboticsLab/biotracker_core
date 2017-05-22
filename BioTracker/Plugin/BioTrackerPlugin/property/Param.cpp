#include "Param.h"

#include "helper/StringHelper.h"

Param::Param(void)
{
}

Param::Param(std::string paramName, std::string paramValue)
{
	setPName(paramName);
	setPValue(paramValue);
}

Param::~Param(void)
{
}
