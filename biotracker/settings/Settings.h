#pragma once

#include <type_traits>
#include <boost/filesystem.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "../util/stringTools.h" // (un)escape_non_ascii
#include "StringTranslator.h"
#include "ParamNames.h"

namespace {
template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref>: std::true_type {};
}

class Settings {
  public:
    /**
     * The default constructor.
     */
    explicit Settings();

    /**
     * destructor.
     */
    ~Settings() = default;

    /**
     * Sets the parameter.
     * @param paramName name of the parameter,
     * @param paramValue value of the parameter,
     */
    template <typename T>
    void setParam(std::string const &paramName, T &&paramValue) {
        _ptree.put(paramName, preprocess_value(std::forward<T>(paramValue)));
        boost::property_tree::write_json(CONFIGPARAM::CONFIGURATION_FILE.string(), _ptree);
    }

    /**
     * Sets the vector of values of a parameter.
     * @param paramName name of the parameter,
     * @param paramVector vector of values of the parameter,
     */
    template <typename T>
    void setParam(std::string const &paramName, std::vector<T> &&paramVector) {
        boost::property_tree::ptree subtree;
        for (T &value : paramVector) {
            boost::property_tree::ptree valuetree;
            valuetree.put("", value);
            subtree.push_back(std::make_pair("", valuetree));
        }
        _ptree.put_child(paramName, subtree);
        boost::property_tree::write_json(CONFIGPARAM::CONFIGURATION_FILE.string(), _ptree);
    }

    /**
     * Gets the parameter value provided by parameter name.
     * @param paramName the parameter name,
     * @return the value of the parameter as the specified type.
     */
    template <typename T>
    typename std::enable_if<!is_specialization<T, std::vector>::value, T>::type
    getValueOfParam(const std::string &paramName) const {
        return postprocess_value(_ptree.get<T>(paramName));
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
        T result;
        for (auto &item : _ptree.get_child(paramName)) {
            result.push_back(postprocess_value(
                                 item.second.get_value<typename T::value_type>()));
        }
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
    T getValueOrDefault(const std::string &paramName, const T &defaultValue) {
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
    return escape_non_ascii(paramValue);
}

/**
 * std::string specialisation as a workaround for a bug in boost's config tree
 *
 * (the tree converts escaped non-ASCII characters ("\u00XX") to "\xFF")
 *
 */
template<>
inline std::string Settings::postprocess_value(std::string &&paramValue) {
    return unescape_non_ascii(paramValue);
}
