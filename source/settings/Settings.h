#pragma once

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "source/settings/ParamNames.h"

class Settings
{
public:
	/**
	 * The default constructor.
	 */
	Settings();

	/**
	 * destructor.
	 */
	~Settings() {}

	/**
	 * Sets the parameter.
	 * @param: paramName, name of the parameter,
	 * @param: paramValue, value of the parameter,
	 * @return: void.
	 */
	template <typename T>
	void setParam(std::string const& paramName, T&& paramValue) {
		_ptree.put<T>(paramName, std::move(paramValue));
		boost::property_tree::write_ini(CONFIGPARAM::CONFIGURATION_FILE, _ptree);
	}

	/**
	 * Gets the parameter value provided by parameter name.
	 * @param: paramName, the parameter name,
	 * @return: the value of the parameter as the specified type.
	 */
	template <typename T>
	T getValueOfParam(const std::string &paramName) const {
		return _ptree.get<T>(paramName);
	}

private:
	boost::property_tree::ptree _ptree;

	static const boost::property_tree::ptree getDefaultParams();
};
