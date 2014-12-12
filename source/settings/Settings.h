#pragma once

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "EnumTranslator.h"
#include "ParamNames.h"

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
	 * @param paramName name of the parameter,
	 * @param paramValue value of the parameter,
	 */
	template <typename T>
	void setParam(std::string const& paramName, T&& paramValue) {
		_ptree.put(paramName, std::forward<T>(paramValue));
		boost::property_tree::write_json(CONFIGPARAM::CONFIGURATION_FILE, _ptree);
	}

	template <typename T>
	void setParamVector(std::string const& paramName, std::vector<T>&& paramVector) {
		boost::property_tree::ptree subtree;
		for (T const& value : paramVector) {
			subtree.push_back(std::make_pair("", boost::property_tree::ptree(value)));
		}
		_ptree.put_child(paramName, subtree);
		boost::property_tree::write_json(CONFIGPARAM::CONFIGURATION_FILE, _ptree);
	}

	/**
	 * Gets the parameter value provided by parameter name.
	 * @param paramName the parameter name,
	 * @return the value of the parameter as the specified type.
	 */
	template <typename T>
	T getValueOfParam(const std::string &paramName) const {
		return _ptree.get<T>(paramName);
	}

	template <typename T>
	std::vector<T> getVectorOfParam(const std::string &paramName) const {
		std::vector<T> result;
		for (auto& item : _ptree.get_child(paramName)) {
			result.push_back(item.second.get_value<T>());
		}
//		auto& subtr
//		auto it = _ptree.find(paramName);
//		std::vector<T> result;
//		for (auto& subtreeit : it->second) {
//			result.push_back(subtreeit.second.get_value<T>());
//		}
		return result;
	}

	/**
	 * Gets either the parameter value provided by parameter name, if it
	 * exists, or a empty boost::optional<T> otherwise.
	 * @param paramName the parameter name,
	 * @return the value of the parameter wrapped in a boost::optional.
	 */
	template <typename T>
	boost::optional<T> maybeGetValueOfParam(const std::string &paramName) const {
		return _ptree.get_optional<T>(paramName);
	}

	/**
	 * Gets the parameter value provided by parameter name.
	 * If the parameter is not set yet, set to default value and return it.
	 * @param paramName the parameter name,
	 * @param defaultValue the default parameter value,
	 * @return the value of the parameter as the specified type.
	 */
	template <typename T>
	T getValueOrDefault(const std::string& paramName, const T& defaultValue) {
		boost::optional<T> value = maybeGetValueOfParam<T>(paramName);
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
