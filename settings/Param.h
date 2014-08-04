#ifndef Param_h
#define Param_h

#include <iostream>
#include <vector>
#include <QtCore/QString>

using namespace std;

class Param {
public:
	Param(void);

	Param(string paramName, string paramValue);

	~Param(void);

	/**
	* Sets the parameter name.
	* @param: paramName, the parameter name as standard string,
	* @return: void.
	*/
	void setPName(string paramName) { _paramName = paramName; }

	/**
	* Sets the parameter value.
	* @param: paramValue, the parameter name as standard string,
	* @return: void.
	*/
	void setPValue(string paramValue) { _paramValue = paramValue; }

	/**
	* Gets the parameter name.
	* return the parameter name as standard string.
	*/
	string pName() { return _paramName; }

	/**
	* Gets the value of the parameter.
	* return the parameter value as standard string.
	*/
	string pValue() { return _paramValue; }

private:
	string _paramName;
	string _paramValue;

};  
#endif // !Param_h
