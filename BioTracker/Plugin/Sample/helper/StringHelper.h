#pragma once

#include <iostream>
#include <vector>
#include <QString>

/**
 * String handling helper class
 */
class StringHelper
{
public:
	StringHelper(void) {}
	~StringHelper(void) {}

	/**
	 * This method is used to check whether the full string contains the provided ending.
	 * @param: fullString, the specifies the string which needs to be checked.
	 * @param: ending, string specifes the ending string.
	 * @return: true if ending string is at the end of fullString, otherwise false 
	 */
	static bool hasEnding(std::string const &fullString, std::string const &ending);

	
	static bool startsWithDigitQ(QString fullString);
	static bool startsWithDigit(std::string const &fullString);

	static bool isNumber(QString fullString);

	/**
	 * Convert QString to standard string.
	 * @param: qString, the string to convert,
	 * @return: a standard string.
	 */
	static std::string toStdString(QString qString);

	/**
	 * Convert standard string to QString
	 * @param: stdString, the string to convert,
	 * @return: a qt string.
	 */
	static QString toQString(std::string stdString);

	/**
	 * Convert integer to standard string.
	 * @param: int_num, integer number to convert,
	 * @return: a standard string.
	 */
	static std::string iToSS(int int_num);

	/**
	 * Convert a bool to standard string.
	 * @param: bool_value, bool valueto convert,
	 * @return: a standard string.
	 */
	static std::string boolToSS(bool bool_value);

	/**
	* Convert float to standard string.
	* @param: float_num, float number to convert,
	* @return: a standard string.
	*/
	static std::string fToSS(float float_num);

	/**
	 * Convert a given string to lower case string.
	 * @param: stringValue, string to convert,
	 * @return: a standard string.
	 */
	static std::string toLowerCase(std::string stringValue);

	/**
	 * Use from here: http://stackoverflow.com/questions/5888022/split-string-by-single-spaces
	 * Method to split a string by a given character.
	 * @param: txt, a reference to the string, which will be split,
	 * @param: strs, a reference to a vector containing the splitted result,
	 * @param: ch, the split character.
	 **/
	static int split(std::string &txt, std::vector<std::string> &strs, char ch);
};
