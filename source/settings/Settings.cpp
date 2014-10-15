#include "Settings.h"

#include <QMutex>


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
    setQSettingsParam(param);
}


void Settings::setParam(std::string paramName, std::string paramValue)
{
	setParam(_params, paramName, paramValue);
	setQSettingsParam(paramName, paramValue);	
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
	settings.setValue(QString::fromStdString(paramName),QString::fromStdString(paramValue));
}

void Settings::setQSettingsParams(std::vector<TrackerParam::Param> params)
{
	QMutexLocker locker(&paramMutex);
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIG_INI_FILE.c_str()), QSettings::IniFormat);
	for(size_t i = 0; i < params.size(); i++)
	{
		settings.setValue(QString::fromStdString(params.at(i).pName()),QString::fromStdString(params.at(i).pValue()));
	}
}

std::vector<TrackerParam::Param> Settings::getParams()
{
	QMutexLocker locker(&paramMutex);
	return _params;
}

template<> std::string Settings::getValueOfParam(std::string paramName)
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

template<> QString Settings::getValueOfParam(std::string paramName)
{
	QMutexLocker locker(&paramMutex);
    for (TrackerParam::Param const& param : _params)
	{	
        if(param.pName().compare(paramName) == 0)
		{
            return QString::fromStdString(param.pValue());
		}
	}

	throw "Parameter cannot be obtained!";
}

template<> double Settings::getValueOfParam(std::string paramName)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);
	char* endptr;

	double valueAsDouble = strtod(valueAsString.c_str(), &endptr);
    if (*endptr) 
		throw "Value cannot convert to double number!";
    
	return valueAsDouble;
}

template<> float Settings::getValueOfParam(std::string paramName)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);
	char* endptr;

	float valueAsFloat = (float)strtod(valueAsString.c_str(), &endptr);
    if (*endptr) 
		throw "Value cannot convert to double number!";
    
	return valueAsFloat;
}

template<> int Settings::getValueOfParam(std::string paramName)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);
	char* endptr;

	int valueAsInt = (int)strtod(valueAsString.c_str(), &endptr);
    if (*endptr) 
		throw "Value cannot convert to int number!";
    
	return valueAsInt;
}

template<> bool Settings::getValueOfParam(std::string paramName)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);
	std::transform(valueAsString.begin(), valueAsString.end(), valueAsString.begin(), ::tolower);

	if(valueAsString.compare("true") == 0 || valueAsString.compare("1") == 0)
		return true;
    
	return false;
}

template<> cv::Scalar Settings::getValueOfParam(std::string paramName)
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	QMutexLocker locker(&paramMutex);

	cv::Scalar cvScalarValue;
	std::vector<cv::string> stringList;

	int tokens = split(valueAsString,stringList,' ');
	// 255 255 255
	if(tokens == 3)
	{
		for(int i = 0; i < tokens; i++)
		{
            int r = std::stoi(stringList.at(0));
            int g = std::stoi(stringList.at(1));
            int b = std::stoi(stringList.at(2));
			cvScalarValue = cv::Scalar(r,g,b);
		}
	}

	return cvScalarValue;
}

std::vector<TrackerParam::Param> Settings::getDefaultParamsFromQSettings()
{
	QMutexLocker locker(&paramMutex);
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIG_INI_FILE.c_str()), QSettings::IniFormat);	
	//TODO: Hier checken ob Datei vorhanden -> wenn nicht da neu anlegen, default params setzen

	std::vector<TrackerParam::Param> defaultParams;

	for(int key = 0; key < settings.allKeys().size(); key++)
	{
		QString string = settings.allKeys().at(key);
		defaultParams.push_back(TrackerParam::Param(string.toStdString(),settings.value(string).toString().toStdString()));
	}	
	return defaultParams;
}

int Settings::split(std::string &txt, std::vector<std::string> &strs, char ch)
{
    std::size_t pos = txt.find( ch );
    std::size_t initialPos = 0;
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

