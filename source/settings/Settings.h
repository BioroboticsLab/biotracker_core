#pragma once

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
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
		_ptree.put(paramName, std::forward<T>(paramValue));
		boost::property_tree::write_json(CONFIGPARAM::CONFIGURATION_FILE, _ptree);
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

	/**
	 * Gets the parameter value provided by parameter name.
	 * If the parameter is not set yet, set to default value and return it.
	 * @param: paramName, the parameter name,
	 * @param: paramDefault, the default parameter value,
	 * @return: the value of the parameter as the specified type.
	 */
	template <typename T>
	T getValueOrDefault(const std::string& paramName, const T& defaultValue) {
		boost::optional<T> value = _ptree.get_optional<T>(paramName);
		if (value) {
			return value.get();
		} else {
			setParam(paramName, defaultValue);
			return defaultValue;
		}
	}

private:
	boost::property_tree::ptree _ptree;

	static const boost::property_tree::ptree getDefaultParams();
};
