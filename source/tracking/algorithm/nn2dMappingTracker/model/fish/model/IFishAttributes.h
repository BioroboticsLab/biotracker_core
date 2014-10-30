#pragma once

#include <map>
#include <string>

class IFishAttributes
{
public:
	
	IFishAttributes(
		std::map<const std::string, bool> _boolAttributes = std::map<const std::string, bool>(),
		std::map<const std::string, int>  _intAttributes = std::map<const std::string, int>(),
		std::map<const std::string, float>  _floatAttributes = std::map<const std::string, float>(),
		std::map<const std::string, double>  _doubleAttributes = std::map<const std::string, double>()
		);

	virtual ~IFishAttributes(void);

	virtual void initVariables() = 0;

	/**
	 * This dynamic generic Getter-function gets the value of a provided variable name.
	 * the variable are not need to be declared in either the cpp or h. They are just 
	 * declared as dynamic variable, but the names are specified as const std::string
	 * with their namespaces in the cpp file.
	 * @param: attrName, the name of the variable.
	 * @param: value, the reference return value.
	 * @return the value of the variable.
	 */
	template<typename T> T getAttribute(const std::string attrName, T &value);
	/*{
		T t;

		std::string tName = typeid(T).name();

		if(tName.compare("bool") == 0) 
		{
			for( std::map<std::string, bool>::iterator ii=_boolAttributes.begin(); ii!=_boolAttributes.end(); ++ii)
			{
				if((*ii).first.compare(attrName) == 0)
				{
					t = (*ii).second;
					break;
				}

			}
		} else if(tName.compare("int") == 0) 
		{
			for( std::map<std::string, int>::iterator ii=_intAttributes.begin(); ii!=_intAttributes.end(); ++ii)
			{
				if((*ii).first.compare(attrName) == 0)
				{
					t = (*ii).second;
					break;
				}

			}
		} else if(tName.compare("float") == 0) 
		{
			for( std::map<std::string, float>::iterator ii=_floatAttributes.begin(); ii!=_floatAttributes.end(); ++ii)
			{
				if((*ii).first.compare(attrName) == 0)
				{
					t = (*ii).second;
					break;
				}

			}
		} else if(tName.compare("double") == 0) 
		{
			for( std::map<std::string, double>::iterator ii=_doubleAttributes.begin(); ii!=_doubleAttributes.end(); ++ii)
			{
				if((*ii).first.compare(attrName) == 0)
				{
					t = (*ii).second;
					break;
				}

			}
		}
		
		return t;
	}*/

	/**
	 * This dynamic generic Setter-function sets the value for a provided variable.
	 * This is the countetpart function of the getter function.
	 * @param: attrName, the name of the variable.
	 * @param: attrValue, the value for thisthe variable.
	 * @return void.
	 */
	template<typename T> void setAttribute(const std::string attrName, T attrValue);
	/*{
		std::string tName = typeid(T).name();

		if(tName.compare("bool") == 0) 
		{
			_boolAttributes[attrName] = attrValue;
			return;
		}
		
		if(tName.compare("int") == 0) 
		{
			_intAttributes[attrName] = attrValue;
			return;
		}

		if(tName.compare("float") == 0) 
		{
			_floatAttributes[attrName] = attrValue;
			return;
		}

		if(tName.compare("double") == 0) 
		{
			_doubleAttributes[attrName] = attrValue;
			return;
		}
	}*/

	/**
	 * Return the attribute types.
	 */
	std::map<const std::string, bool> boolAttributes() { return _boolAttributes; }
	std::map<const std::string, int> intAttributes() { return _intAttributes; }
	std::map<const std::string, float> floatAttributes() { return _floatAttributes; }
	std::map<const std::string, double> doubleAttributes() { return _doubleAttributes; }


protected:
	// various attributes of different data types.
	std::map<const std::string, bool> _boolAttributes;
	std::map<const std::string, int>  _intAttributes;
	std::map<const std::string, float>  _floatAttributes;
	std::map<const std::string, double>  _doubleAttributes;

};

