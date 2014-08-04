#ifndef Settings_H
#define Settings_H

#include <iostream>
#include <vector>
#include <regex>

#include <cv.h>

#include <QtCore/QSettings>
#include <QtCore/QString>

#include "ParamNames.h"
#include "Param.h"

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
	Settings(std::vector<Param> params);

	/**
	* The standard destructor.
	*/
	~Settings(void);

	/**
	* Sets the parameter.
	* @param: param, the parameter to set,
	* @return: void.
	*/
	void setParam(Param param);	

	/**
	* Sets the parameter.
	* @param: paramName, name of the parameter,
	* @param: paramValue, value of the parameter,
	* @return: void.
	*/
	void setParam(std::string paramName, std::string paramValue);

	/**
	* Sets the parameter. Generic method.
	* @param: paramName, name of the parameter,
	* @param: value, value of the parameter,
	* @return: void.
	*/
	template <typename T> void setParam2(std::string paramName, T value);

	/**
	* Sets the parameters.
	* @param: params, the parameter list to set,
	* @return: void.
	*/
	void setParams(std::vector<Param> params);

	/**
	* Sets a parameter within a parameter vector
	* @param: params, the parameter vector,
	* @param: param, the parameter to set,
	* @return: void.
	*/
	void setParam(std::vector<Param> &params, Param param);

	/**
	* Sets a parameter within a parameter vector
	* @param: params, the parameter vector,
	* @param: paramName, the parameter name to set,
	* @param: paramValue, the parameter value to set,
	* @return: void.
	*/
	void setParam(std::vector<Param> &params, std::string paramName, std::string paramValue);

	/**
	* Sets the parameter for the config.ini file.
	* @param: param, the parameter to set.
	* @return: void.
	*/
	void setQSettingsParam(Param param);

	/**
	* Sets the parameter for the config.ini file.
	* @param: paramName, the parameter name.
	* @param: paramValue, the parameter value.
	* @return: void.
	*/
	void setQSettingsParam(std::string paramName, std::string paramValue);

	/**
	* Sets the parameters for the config.ini file (use QSettings class).
	* @param: params, the parameter list to set,
	* @return: void.
	*/
	void setQSettingsParams(std::vector<Param> params);

	/**
	* Gets the parameters.
	* @return: a vector containing all tracking parameters.
	*/
	std::vector<Param> getParams();

	/**
	* Gets the parameter value provided by parameter name. 
	* @param: paramName, the parameter name,
	* @param: size, if the parameter is a list or vector,
	*				 then one can provide the length of the list.
	* @return: the value of the parameter as the specified type.
	*/
	template <typename T> T getValueOfParam(std::string paramName, int size = 4);

	/** 
	* Initialize the tracker with default parameters, from the config.ini file.
	* @return a vector contains track parameters.
	*/
	static std::vector<Param> getDefaultParamsFromQSettings();

private:
	std::vector<Param> _params;
};  
#endif // !Settings_H
