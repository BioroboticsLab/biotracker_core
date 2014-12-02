#pragma once

#pragma warning(push, 0)   
#pragma warning(disable:4503)
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#pragma warning(pop)

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
#pragma warning(push, 0)   
#pragma warning(disable:4503)
		_ptree.put<T>(paramName, std::forward<T>(paramValue));
		boost::property_tree::write_ini(CONFIGPARAM::CONFIGURATION_FILE, _ptree);
#pragma warning(pop)
	}

	/**
	 * Gets the parameter value provided by parameter name.
	 * @param: paramName, the parameter name,
	 * @return: the value of the parameter as the specified type.
	 */
	template <typename T>
	T getValueOfParam(const std::string &paramName) const {
#pragma warning(push, 0)   
#pragma warning(disable:4503)
		return _ptree.get<T>(paramName);
#pragma warning(pop)
	}

private:
	boost::property_tree::ptree _ptree;

	static const boost::property_tree::ptree getDefaultParams();
};
