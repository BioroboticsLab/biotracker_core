#pragma once

#include <iostream>
#include <vector>
#include <QtCore/QString>

namespace TrackerParam{
class Param
{
public:
	/**
	 * The standard constructor.
	 */
	Param(void);

	Param(std::string paramName, std::string paramValue);

	~Param(void);

	/**
	 * Sets the parameter name.
	 * @param: paramName, the parameter name as standard string,
	 * @return: void.
	 */
	void setPName(std::string paramName) { _paramName = paramName; }

	/**
	 * Sets the parameter value.
	 * @param: paramValue, the parameter name as standard string,
	 * @return: void.
	 */
	void setPValue(std::string paramValue) { _paramValue = paramValue; }

	/**
	 * Gets the parameter name.
	 * return the parameter name as standard string.
	 */
	std::string pName() { return _paramName; }

	/**
	 * Gets the value of the parameter.
	 * return the parameter value as standard string.
	 */
	std::string pValue() { return _paramValue; }

private:
	std::string _paramName;
	std::string _paramValue;
};
}
