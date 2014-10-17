#include "Param.h"

namespace TrackerParam {

Param::Param(void)
{
}

Param::Param(std::string paramName, std::string paramValue)
  : _paramName(std::move(paramName))
  , _paramValue(std::move(paramValue))
{
}

Param::~Param(void)
{
}

}