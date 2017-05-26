#pragma once

#include <string>

class IParam
{
public:
	

	virtual ~IParam(void){};

	/**
	 * Sets specified parameter with an integer value.
	 * @param: spec_param, the specified parameter to set,
	 * @param: value, the value to set,
	 * @return: void.
	 */
	virtual void setInt(std::string spec_param, int value) = 0;

	/**
	 * Sets specified parameter with a boolean value.
	 * @param: spec_param, the specified parameter to set,
	 * @param: value, the boolean value to set,
	 * @return: void.
	 */
	virtual void setBool(std::string spec_param, bool value) = 0;

	/**
	 * Sets specified parameter with a float value.
	 * @param: spec_param, the specified parameter to set,
	 * @param: value, the float value to set,
	 * @return: void.
	 */
	virtual void setFloat(std::string spec_param, float value) = 0;

	/**
	 * Sets specified parameter with a double value.
	 * @param: spec_param, the specified parameter to set,
	 * @param: value, the double value to set,
	 * @return: void.
	 */
	virtual void setDouble(std::string spec_param, double value) = 0;

	/**
    * This dynamic generic Getter-function gets the value of a provided variable name.
    * the variable are not need to be declared in either the cpp or h. They are just 
    * declared as dynamic variable, but the names are specified as const std::string
    * with their namespaces in the cpp file.
    * @param: attrName, the name of the variable.
    * @param: value, the reference return value.
    * @return the value of the variable.
    */
    template<typename T> T get(const std::string attrName, T &value);

	/**
    * This dynamic generic Setter-function sets the value for a provided variable.
    * This is the countetpart function of the getter function.
    * @param: attrName, the name of the variable.
    * @param: attrValue, the value for thisthe variable.
    * @return void.
    */
	template<typename T> void set(const std::string attrName, T attrValue);
};

