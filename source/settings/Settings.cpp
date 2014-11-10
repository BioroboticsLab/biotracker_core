#include "Settings.h"

#include <QMutex>


/**
* Mutexes.
*/
namespace {
  QMutex paramMutex;
}

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
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIGURATION_FILE.c_str()), QSettings::IniFormat);
	settings.setValue(QString::fromStdString(paramName),QString::fromStdString(paramValue));
}

void Settings::setQSettingsParams(std::vector<TrackerParam::Param> params)
{
	QMutexLocker locker(&paramMutex);
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIGURATION_FILE.c_str()), QSettings::IniFormat);
	for(size_t i = 0; i < params.size(); i++)
	{
		settings.setValue(QString::fromStdString(params.at(i).pName()),QString::fromStdString(params.at(i).pValue()));
	}
}

std::vector<TrackerParam::Param> Settings::getParams() const
{
	QMutexLocker locker(&paramMutex);
	return _params;
}

template<> std::string Settings::getValueOfParam(std::string paramName) const
{
	QMutexLocker locker(&paramMutex);
    for (TrackerParam::Param const& param : _params)
	{	
        if(param.pName().compare(paramName) == 0)
		{
            return param.pValue();
		}
	}

    throw std::invalid_argument("Parameter cannot be obtained");
}

template<> QString Settings::getValueOfParam(std::string paramName) const
{
    return QString::fromStdString(getValueOfParam<std::string>(paramName));
}

template<> double Settings::getValueOfParam(std::string paramName) const
{
    const std::string valueAsString = getValueOfParam<std::string>(paramName);
    return std::stod(valueAsString);
}

template<> float Settings::getValueOfParam(std::string paramName) const
{
    const std::string valueAsString = getValueOfParam<std::string>(paramName);
    return std::stof(valueAsString);
}

template<> int Settings::getValueOfParam(std::string paramName) const
{
    const std::string valueAsString = getValueOfParam<std::string>(paramName);
    return std::stoi(valueAsString);
}

template<> bool Settings::getValueOfParam(std::string paramName) const
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	std::transform(valueAsString.begin(), valueAsString.end(), valueAsString.begin(), ::tolower);

    if(valueAsString == "true" || valueAsString == "1")
		return true;
    
	return false;
}

template<> cv::Scalar Settings::getValueOfParam(std::string paramName) const
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	const std::vector<cv::string>& stringList = Settings::split(valueAsString, ' ');

    if(stringList.size() != 3)
	{
        throw std::invalid_argument("Number of tokens must be 3");
    }

    int r = std::stoi(stringList.at(0));
    int g = std::stoi(stringList.at(1));
    int b = std::stoi(stringList.at(2));

    return cv::Scalar(r,g,b);
}

std::vector<TrackerParam::Param> Settings::getDefaultParamsFromQSettings()
{
	QMutexLocker locker(&paramMutex);
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIGURATION_FILE.c_str()), QSettings::IniFormat);	
	//TODO: Hier checken ob Datei vorhanden -> wenn nicht da neu anlegen, default params setzen

	std::vector<TrackerParam::Param> defaultParams;

    for (QString const& key : settings.allKeys())
    {
        defaultParams.emplace_back(key,settings.value(key).toString());
	}	
	return defaultParams;
}

std::vector<std::string> Settings::split(const std::string &txt, char ch)
{
    std::vector<std::string> result;
    std::size_t pos = txt.find( ch );
    std::size_t initialPos = 0;

    // Decompose statement
    while( pos != std::string::npos ) {
        result.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    result.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return result;
}

