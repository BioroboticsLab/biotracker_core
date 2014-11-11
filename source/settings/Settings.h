#pragma once

#include <iostream>
#include <vector>
#include <regex>

#include <cv.h>

#include <QtCore/QSettings>
#include <QtCore/QString>

#include "source/settings/ParamNames.h"
#include "source/settings/Param.h"

class Settings
{
public:
	/**
	 * The standard constructor.
	 */
	Settings(void);

	/**
	 * The constructor with provided parameters.
	 * @param params, parameter property.
	 */
	Settings(std::vector<TrackerParam::Param> params);

	/**
	 * The standard destructor.
	 */
	~Settings(void);

	/**
	 * Sets the parameter.
	 * @param: paramName, name of the parameter,
	 * @param: paramValue, value of the parameter,
	 * @return: void.
	 */
	void setParam(std::string paramName, std::string paramValue);

	/**
	 * Sets the parameters.
	 * @param: params, the parameter list to set,
	 * @return: void.
	 */
	void setParams(std::vector<TrackerParam::Param> params);

	/**
	 * Gets the parameters.
	 * @return: a vector containing all tracking parameters.
	 */
	std::vector<TrackerParam::Param> getParams() const;

	/**
	 * Gets the parameter value provided by parameter name. 
	 * @param: paramName, the parameter name,
	 * @return: the value of the parameter as the specified type.
	 */
    template <typename T> T getValueOfParam(const std::string &paramName) const;

	/** 
	 * Initialize the tracker with default parameters, from the config.ini file.
	 * @return a vector contains track parameters.
	 */
	static std::vector<TrackerParam::Param> getDefaultParamsFromQSettings();
	
private:

	/**
	 * Sets a parameter within this' parameter vector
	 * This helper method is NOT threadsafe!
	 * @param: paramName, the parameter name to set,
	 * @param: paramValue, the parameter value to set,
	 * @return: void.
	 */
	void setParamInVector(std::string paramName, std::string paramValue);

	/**
	 * Sets the parameter for the config.ini file.
	 * This helper method is NOT threadsafe!
	 * @param: paramName, the parameter name.
	 * @param: paramValue, the parameter value.
	 * @return: void.
	 */
	void setParamInConfigFile(const std::string &paramName, const std::string &paramValue);

	std::vector<TrackerParam::Param> _params;
	static std::vector<std::string> split(const std::string &txt, char ch);
};
