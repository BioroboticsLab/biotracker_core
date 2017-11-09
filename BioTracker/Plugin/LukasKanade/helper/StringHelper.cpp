#include "StringHelper.h"

#include <algorithm>
#include <string>


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

bool StringHelper::startsWithDigitQ(QString fullString)
{
	return StringHelper::startsWithDigit(fullString.toLocal8Bit().data());
}

bool StringHelper::isNumber(QString fullStringQ)
{	
	std::string fullString = fullStringQ.toLocal8Bit().data();

	for(int i = 0; i < fullString.length(); i++)
	{
		if(isdigit(fullString.at(i)))
		{
			return true;
		}

	}

	return false;
}

std::string StringHelper::iToSS(int int_num)
{
	return QString::number(int_num).toUtf8().constData();
}

std::string StringHelper::boolToSS(bool bool_value)
{
	if(bool_value)
		return std::string("true");
	return std::string("false");
}

std::string StringHelper::fToSS(float float_num)
{
	return QString::number(float_num).toUtf8().constData();
}


std::string StringHelper::toStdString(QString qString)
{
	return qString.toUtf8().constData();
}

QString StringHelper::toQString(std::string stdString)
{
	return QString::fromUtf8(stdString.c_str());
}

std::string StringHelper::toLowerCase(std::string stringValue)
{
	std::string lowCaseString(stringValue);
	std::transform(lowCaseString.begin(), lowCaseString.end(), lowCaseString.begin(), tolower);
	return lowCaseString;
}

int StringHelper::split(std::string &txt, std::vector<std::string> &strs, char ch)
{
    std::string::size_type pos = txt.find( ch );
	std::string::size_type initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

    return strs.size();
}