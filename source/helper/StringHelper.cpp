#include "StringHelper.h"

#include <algorithm>
#include <string>
#include <utility> // std::move

bool StringHelper::hasEnding(std::string const &fullString, std::string const &ending)
{
	if (fullString.length() >= ending.length())
		return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
	else
		return false;
}


bool StringHelper::startsWithDigit(std::string const &fullString)
{
	if(fullString.empty())
		return false;


	if(isdigit(fullString.at(0)))
	{
		return true;
	}

	return false;

}

bool StringHelper::startsWithDigitQ(const QString &fullString)
{
	return StringHelper::startsWithDigit(fullString.toLocal8Bit().data());
}

bool StringHelper::isNumber(const QString &fullStringQ)
{	
	const std::string fullString = fullStringQ.toLocal8Bit().data();
	return std::any_of(fullString.cbegin(), fullString.cend(), isdigit);
}

std::string StringHelper::iToSS(int int_num)
{
	return QString::number(int_num).toStdString();
}

std::string StringHelper::boolToSS(bool bool_value)
{
	if(bool_value)
		return std::string("true");
	return std::string("false");
}

std::string StringHelper::toStdString(const QString &qString)
{
	return qString.toStdString();
}

QString StringHelper::toQString(const std::string &stdString)
{
	return QString::fromUtf8(stdString.c_str());
}

std::string StringHelper::toLowerCase(std::string stringValue)
{
	std::transform(stringValue.begin(), stringValue.end(), stringValue.begin(), tolower);
	return std::move(stringValue);
}

int StringHelper::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}
