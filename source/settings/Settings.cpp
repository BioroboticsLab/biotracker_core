#include "Settings.h"

#include <QMutex>

#include "source/helper/StringHelper.h"
#include "source/helper/CvHelper.h"

/**
* Mutexes.
*/
QMutex paramMutex;

Settings::Settings(void) :
	_params(getDefaultParamsFromQSettings())

{
}

Settings::Settings(std::vector<TrackerParam::Param> params) :
	_params(params)
{

}

Settings::~Settings(void)
{
}

void Settings::setParam(TrackerParam::Param param)
{
    setParam(_params,param);
	setQSettingsParam(param);
}


void Settings::setParam(std::string paramName, std::string paramValue)
{
	setParam(_params, paramName, paramValue);
	setQSettingsParam(paramName, paramValue);	
}

template <> void Settings::setParam2(std::string paramName, bool value)
{
	//NOT IMPLEMENT YET
}

template <> void Settings::setParam2(std::string paramName, int value)
{
	//NOT IMPLEMENT YET
}

template <> void Settings::setParam2(std::string paramName, float value)
{
	//NOT IMPLEMENT YET
}

template <> void Settings::setParam2(std::string paramName, double value)
{
	//NOT IMPLEMENT YET
}

template <> void Settings::setParam2(std::string paramName, std::string value)
{
	//NOT IMPLEMENT YET
}

template <> void Settings::setParam2(std::string paramName, QString value)
{
	//NOT IMPLEMENT YET
}



void Settings::setParam(std::vector<TrackerParam::Param> &params, TrackerParam::Param param)
{
	setParam(_params, param.pName(), param.pValue());
}

void Settings::setParam(std::vector<TrackerParam::Param> &params, std::string paramName, std::string paramValue)
{
	QMutexLocker locker(&paramMutex);
	bool found = false;
    for (TrackerParam::Param& param : params)
	{	
        if(param.pName().compare(paramName) == 0)
		{
            param.setPValue(paramValue);
			found = true;
			break;
		}
	}
	if(found == false)
		params.push_back(TrackerParam::Param(paramName,paramValue));
}

void Settings::setQSettingsParam(TrackerParam::Param param)
{
	setQSettingsParam(param.pName(),param.pValue());
}

void Settings::setQSettingsParam(std::string paramName, std::string paramValue)
{
	QMutexLocker locker(&paramMutex);
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIG_INI_FILE.c_str()), QSettings::IniFormat);
	settings.setValue(StringHelper::toQString(paramName),StringHelper::toQString(paramValue));
}

void Settings::setQSettingsParams(std::vector<TrackerParam::Param> params)
{
	QMutexLocker locker(&paramMutex);
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIG_INI_FILE.c_str()), QSettings::IniFormat);
	for(size_t i = 0; i < params.size(); i++)
	{
		settings.setValue(StringHelper::toQString(params.at(i).pName()),StringHelper::toQString(params.at(i).pValue()));
	}
}

std::vector<TrackerParam::Param> Settings::getParams()
{
	QMutexLocker locker(&paramMutex);
	return _params;
}

template<> std::string Settings::getValueOfParam(std::string paramName, int size)
{
	QMutexLocker locker(&paramMutex);
    for (TrackerParam::Param const& param : _params)
	{	
        if(param.pName().compare(paramName) == 0)
		{
            return param.pValue();
		}
	}

	throw "Parameter cannot be obtained!";
}

template<> QString Settings::getValueOfParam(std::string paramName, int size)
{
	QMutexLocker locker(&paramMutex);
    for (TrackerParam::Param const& param : _params)
	{	
        if(param.pName().compare(paramName) == 0)
		{
            return StringHelper::toQString(param.pValue());
		}
	}

	throw "Parameter cannot be obtained!";
}

template<> double Settings::getValueOfParam(std::string paramName, int size)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);
	char* endptr;

	double valueAsDouble = strtod(valueAsString.c_str(), &endptr);
    if (*endptr) 
		throw "Value cannot convert to double number!";
    
	return valueAsDouble;
}

template<> float Settings::getValueOfParam(std::string paramName, int size)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);
	char* endptr;

	float valueAsFloat = (float)strtod(valueAsString.c_str(), &endptr);
    if (*endptr) 
		throw "Value cannot convert to double number!";
    
	return valueAsFloat;
}

template<> int Settings::getValueOfParam(std::string paramName, int size)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);
	char* endptr;

	int valueAsInt = (int)strtod(valueAsString.c_str(), &endptr);
    if (*endptr) 
		throw "Value cannot convert to int number!";
    
	return valueAsInt;
}

template<> bool Settings::getValueOfParam(std::string paramName, int size)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);

	valueAsString = StringHelper::toLowerCase(valueAsString);

	if(valueAsString.compare("true") == 0 || valueAsString.compare("1") == 0)
		return true;
    
	return false;
}

template<> cv::Scalar Settings::getValueOfParam(std::string paramName, int size)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);

	cv::Scalar cvScalarValue;
	std::vector<cv::string> stringList;

	int tokens = StringHelper::split(valueAsString,stringList,' ');
	// 255 255 255
	if(tokens == 3)
	{
		for(int i = 0; i < tokens; i++)
		{
			int r = CvHelper::stdStringToInt(stringList.at(0));
			int g = CvHelper::stdStringToInt(stringList.at(1));
			int b = CvHelper::stdStringToInt(stringList.at(2));
			cvScalarValue = cv::Scalar(r,g,b);
		}
	}

	return cvScalarValue;
}

template<> std::vector<cv::Point> Settings::getValueOfParam(std::string paramName, int size)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);

	std::vector<cv::Point> pointList;
	std::vector<cv::string> pointStringList;

	int tokens = StringHelper::split(valueAsString,pointStringList,' ');
	// 151,454-152,26-562,32-572,446
	if(tokens == size)
	{
		std::vector<std::string> pstrings;
		for(int i = 0; i < tokens; i++)
		{
			int p = StringHelper::split(pointStringList.at(i),pstrings,':');
			if(p != 2)
			{
				break;
			} else 
			{
				int x = CvHelper::stdStringToInt(pstrings.at(0));
				int y = CvHelper::stdStringToInt(pstrings.at(1));
				pointList.push_back(cv::Point(x,y));
			}
		}
	}

	if(pointList.size() != size)
		return std::vector<cv::Point>();
	return pointList;
}

std::vector<TrackerParam::Param> Settings::getDefaultParamsFromQSettings()
{
	QMutexLocker locker(&paramMutex);
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIG_INI_FILE.c_str()), QSettings::IniFormat);	
	std::vector<TrackerParam::Param> defaultParams;

	for(int key = 0; key < settings.allKeys().size(); key++)
	{
		QString string = settings.allKeys().at(key);
		defaultParams.push_back(TrackerParam::Param(string.toStdString(),settings.value(string).toString().toStdString()));
	}	
	return defaultParams;
}

