#pragma once

#include <type_traits>
#include <boost/filesystem.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "../util/stringTools.h" // (un)escape_non_ascii
#include "StringTranslator.h"
#include "ParamNames.h"
#include <mutex>
#include "util/singleton.h"

#define GET_CORESETTINGS	BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance

namespace {
template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref>: std::true_type {};
}

namespace BioTracker {
namespace Core {

class Settings {
private:
	std::string _confFile;
	std::mutex _m;
	std::map<std::string, void*> _dataStore;

public:

	/* This is a singleton. Get it using something like:
	* SettingsIAC *myInstance = SettingsIAC::getInstance();
	*/
	Settings(std::string config) {
		//_m.lock();
		//Setting default file, if unset
		if (config == "") _confFile = "config.ini";
		else _confFile = config;

		std::ifstream conf(_confFile.c_str());
		if (conf.good())
		{
			boost::property_tree::read_json(_confFile, _ptree);
		}
		else {
			_ptree = getDefaultParams();
			boost::property_tree::write_json(_confFile, _ptree);
		}
		//_m.unlock();
	}

	void storeValue(std::string key, void* value) {
		_dataStore.insert(std::pair<std::string, void*>(key, value));
	}

	void* readValue(std::string key) {
		std::map<std::string, void*>::iterator it = _dataStore.find(key);
		if (it != _dataStore.end()) {
			return it->second;
		}
		return nullptr;
	}

	// C++ 11 style
	// =======
	Settings(Settings const&) = delete;
	void operator=(Settings const&) = delete;

    /**
     * Sets the parameter.
     * @param paramName name of the parameter,
     * @param paramValue value of the parameter,
     */
    template <typename T>
    void setParam(std::string const &paramName, T &&paramValue) {
		//_m.lock();
        _ptree.put(paramName, preprocess_value(std::forward<T>(paramValue)));
        boost::property_tree::write_json(_confFile, _ptree);
		//_m.unlock();
    }

    /**
     * Sets the vector of values of a parameter.
     * @param paramName name of the parameter,
     * @param paramVector vector of values of the parameter,
     */
    template <typename T>
    void setParam(std::string const &paramName, std::vector<T> &&paramVector) {
		//_m.lock();
        boost::property_tree::ptree subtree;
        for (T &value : paramVector) {
            boost::property_tree::ptree valuetree;
            valuetree.put("", value);
            subtree.push_back(std::make_pair("", valuetree));
        }
        _ptree.put_child(paramName, subtree);
        boost::property_tree::write_json(_confFile, _ptree);
		//_m.unlock();
    }

    /**
     * Gets the parameter value provided by parameter name.
     * @param paramName the parameter name,
     * @return the value of the parameter as the specified type.
     */
    template <typename T>
    typename std::enable_if<!is_specialization<T, std::vector>::value, T>::type
    getValueOfParam(const std::string &paramName) const {
		//_m.lock();
        auto a = postprocess_value(_ptree.get<T>(paramName));
		//_m.unlock();
		return a;
    }

    /**
     * Gets the vector of values provided by parameter name.
     *
     * Throws an boost exception if parameter does not exist.
     *
     * @param paramName the parameter name,
     * @return the vector of values of the parameter with the specified type.
     */
    template <typename T>
    typename std::enable_if<is_specialization<T, std::vector>::value, T>::type
    getValueOfParam(const std::string &paramName) const {
		//_m.lock();
        T result;
        for (auto &item : _ptree.get_child(paramName)) {
            result.push_back(postprocess_value(
                                 item.second.get_value<typename T::value_type>()));
        }
		//_m.unlock();
        return result;
    }

    /**
     * Gets either the parameter value provided by parameter name, if it
     * exists, or a empty boost::optional<T> otherwise.
     * @param paramName the parameter name,
     * @return the value of the parameter wrapped in a boost::optional.
     */
    template <typename T>
    boost::optional<T> maybeGetValueOfParam(const std::string &paramName) {
		//_m.lock();
        auto a = _ptree.get_optional<T>(paramName);
		//_m.unlock();
		return a;
    }

    /**
     * Gets the parameter value provided by parameter name.
     * If the parameter is not set yet, set to default value and return it.
     * @param paramName the parameter name,
     * @param defaultValue the default parameter value,
     * @return the value of the parameter as the specified type.
     */
    template <typename T>
    T getValueOrDefault(const std::string &paramName, const T &defaultValue) {
		//_m.lock();
        boost::optional<T> value = maybeGetValueOfParam<T>(paramName);
		T t;
        if (value) {
            t = value.get();
        } else {
            setParam(paramName, defaultValue);
            t = defaultValue;
        }
		//_m.unlock();
		return t;
    }

  private:
    boost::property_tree::ptree _ptree;

    static const boost::property_tree::ptree getDefaultParams();

    /**
     * preprocesses paramValue before it's stored in the boost config tree
     *
     * default implementation: forward value
     *
     */
    template<typename T>
    static T preprocess_value(T &&paramValue) {
        return std::forward<T>(paramValue);
    }

    /**
     * postprocesses paramValue after it's extracted from the boost config tree
     *
     * default implementation: forward value
     *
     */
    template<typename T>
    static T postprocess_value(T &&paramValue) {
        return std::forward<T>(paramValue);
    }

};

/**
 * std::string specialisation as a workaround for a bug in boost's config tree
 *
 * this function escapes every non-ASCII character
 *
 * (the tree correctly escapes non-ASCII characters and stores them
 *  as "\u00XX" where XX is the character's hex value, but it can't read these
 *  values correctly)
 *
 */
template<>
inline std::string Settings::preprocess_value(std::string &&paramValue) {
    return Util::escape_non_ascii(paramValue);
}

/**
 * std::string specialisation as a workaround for a bug in boost's config tree
 *
 * (the tree converts escaped non-ASCII characters ("\u00XX") to "\xFF")
 *
 */
template<>
inline std::string Settings::postprocess_value(std::string &&paramValue) {
    return Util::unescape_non_ascii(paramValue);
}

}
}
