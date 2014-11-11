#include "Settings.h"

#include <QMutex>
#include <algorithm> // std::find_if
#include <utility> // std::move

namespace {
	QMutex paramMutex;

	struct ParamNameCompare {
		const std::string &name;
		bool operator()(const TrackerParam::Param &p) const {
			return p.pName() == name;
		}
	};
}

Settings::Settings() :
	_params(getDefaultParamsFromQSettings())

{
}

Settings::Settings(std::vector<TrackerParam::Param> params) :
	_params(std::move(params))
{
}

Settings::~Settings() = default;

void Settings::setParam(std::string paramName, std::string paramValue)
{
	QMutexLocker locker(&paramMutex);
	setParamInVector(paramName, paramValue);
	setParamInConfigFile(paramName, paramValue);
}

void Settings::setParamInVector(std::string paramName, std::string paramValue)
{
	const auto pos = std::find_if(_params.begin(), _params.end(), ParamNameCompare{paramName});
	if (pos != _params.end()) {
		pos->setPValue(std::move(paramValue));
	}
	else {
		_params.emplace_back(std::move(paramName), std::move(paramValue));
	}
}

void Settings::setParamInConfigFile(const std::string &paramName, const std::string &paramValue)
{
	QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIGURATION_FILE.c_str()), QSettings::IniFormat);
	settings.setValue(QString::fromStdString(paramName),QString::fromStdString(paramValue));
}

std::vector<TrackerParam::Param> Settings::getParams() const
{
	QMutexLocker locker(&paramMutex);
	return _params;
}

template<> std::string Settings::getValueOfParam(const std::string &paramName) const
{
	QMutexLocker locker(&paramMutex);
	const auto pos = std::find_if(_params.cbegin(), _params.cend(), ParamNameCompare{paramName});
	if (pos != _params.cend()) {
		return pos->pValue();
	}
	else {
		throw std::invalid_argument("Parameter cannot be obtained");
	}
}

template<> QString Settings::getValueOfParam(const std::string &paramName) const
{
	return QString::fromStdString(getValueOfParam<std::string>(paramName));
}

template<> double Settings::getValueOfParam(const std::string &paramName) const
{
	const std::string valueAsString = getValueOfParam<std::string>(paramName);
	return std::stod(valueAsString);
}

template<> float Settings::getValueOfParam(const std::string &paramName) const
{
	const std::string valueAsString = getValueOfParam<std::string>(paramName);
	return std::stof(valueAsString);
}

template<> int Settings::getValueOfParam(const std::string &paramName) const
{
	const std::string valueAsString = getValueOfParam<std::string>(paramName);
	return std::stoi(valueAsString);
}

template<> bool Settings::getValueOfParam(const std::string &paramName) const
{
	std::string valueAsString = getValueOfParam<std::string>(paramName);
	std::transform(valueAsString.begin(), valueAsString.end(), valueAsString.begin(), ::tolower);

	if(valueAsString == "true" || valueAsString == "1") {
		return true;
	}
	return false;
}

template<> cv::Scalar Settings::getValueOfParam(const std::string &paramName) const
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

