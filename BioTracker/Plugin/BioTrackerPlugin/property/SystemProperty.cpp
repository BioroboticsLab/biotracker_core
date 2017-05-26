#include "SystemProperty.h"

#include "helper/StringHelper.h"
#include "helper/CvHelper.h"
#include <QMutex>

/**
* Mutexes.
*/
QMutex paramMutex;

SystemProperty::SystemProperty(void) :
    _params(getDefaultParamsFromQSettings()) {
}

void SystemProperty::init(std::vector<Param> params) {
    _params = params;
}

void SystemProperty::setParam(Param param) {
    setParam(_params, param);
    setQSettingsParam(param);
}

void SystemProperty::setParam(std::string paramName, std::string paramValue) {
    setParam(_params, paramName, paramValue);
    setQSettingsParam(paramName, paramValue);
}

template <> void SystemProperty::setParam2(std::string paramName, bool value) {
    throw std::runtime_error("Function not implmented yet");
}

template <> void SystemProperty::setParam2(std::string paramName, int value) {
    throw std::runtime_error("Function not implmented yet");
}

template <> void SystemProperty::setParam2(std::string paramName, float value) {
    throw std::runtime_error("Function not implmented yet");
}

template <> void SystemProperty::setParam2(std::string paramName, double value) {
    throw std::runtime_error("Function not implmented yet");
}

template <> void SystemProperty::setParam2(std::string paramName, std::string value) {
    throw std::runtime_error("Function not implmented yet");
}

template <> void SystemProperty::setParam2(std::string paramName, QString value) {
    throw std::runtime_error("Function not implmented yet");
}



void SystemProperty::setParam(std::vector<Param>& params, Param param) {
    setParam(_params, param.pName(), param.pValue());
}

void SystemProperty::setParam(std::vector<Param>& params, std::string paramName, std::string paramValue) {
    QMutexLocker locker(&paramMutex);
    bool found = false;
    for (const std::vector<Param>::iterator& it = params.begin(); it != params.end(); ++it) {
        if(it->pName().compare(paramName) == 0) {
            it->setPValue(paramValue);
            found = true;
            break;
        }
    }
    if(found == false)
        params.push_back(Param(paramName, paramValue));
}

void SystemProperty::setQSettingsParam(Param param) {
    setQSettingsParam(param.pName(), param.pValue());
}

void SystemProperty::setQSettingsParam(std::string paramName, std::string paramValue) {
    QMutexLocker locker(&paramMutex);
    QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIG_INI_FILE.c_str()), QSettings::IniFormat);
    settings.setValue(StringHelper::toQString(paramName), StringHelper::toQString(paramValue));
}

void SystemProperty::setQSettingsParams(std::vector<Param> params) {
    QMutexLocker locker(&paramMutex);
    QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIG_INI_FILE.c_str()), QSettings::IniFormat);
    for(int i = 0; i < params.size(); i++) {
        settings.setValue(StringHelper::toQString(params.at(i).pName()), StringHelper::toQString(params.at(i).pValue()));
    }
}

std::vector<Param> SystemProperty::getParams() {
    QMutexLocker locker(&paramMutex);
    return _params;
}

template<> std::string SystemProperty::getValueOfParam(std::string paramName, int size) {
    QMutexLocker locker(&paramMutex);
    for (const std::vector<Param>::iterator& it = _params.begin(); it != _params.end(); ++it) {
        //TODO: we have a problem with the tracking method constant!!!! fix it here please
        if(it->pName().compare(paramName) == 0) {
            return it->pValue();
        }
    }

    throw "Parameter cannot be obtained!";
}

template<> QString SystemProperty::getValueOfParam(std::string paramName, int size) {
    QMutexLocker locker(&paramMutex);
    for (const std::vector<Param>::iterator& it = _params.begin(); it != _params.end(); ++it) {
        if(it->pName().compare(paramName) == 0) {
            return StringHelper::toQString(it->pValue());
        }
    }

    throw "Parameter cannot be obtained!";
}

template<> double SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);
    char* endptr;

    double valueAsDouble = strtod(valueAsString.c_str(), &endptr);
    if (*endptr)
        throw "Value cannot convert to double number!";

    return valueAsDouble;
}

template<> float SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);
    char* endptr;

    float valueAsFloat = (float)strtod(valueAsString.c_str(), &endptr);
    if (*endptr)
        throw "Value cannot convert to double number!";

    return valueAsFloat;
}

template<> int SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);
    char* endptr;

    int valueAsInt = (int)strtod(valueAsString.c_str(), &endptr);
    if (*endptr)
        throw "Value cannot convert to int number!";

    return valueAsInt;
}
template<> size_t SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);
    char* endptr;

    size_t valueAsInt = (size_t)strtoul(valueAsString.c_str(), &endptr, 10);
    if (*endptr)
        throw "Value cannot convert to int number!";

    return valueAsInt;
}


template<> bool SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);

    valueAsString = StringHelper::toLowerCase(valueAsString);

    if(valueAsString.compare("true") == 0 || valueAsString.compare("1") == 0)
        return true;

    return false;
}

template<> cv::Scalar SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);

    cv::Scalar cvScalarValue;
    std::vector<std::string> stringList;

    int tokens = StringHelper::split(valueAsString, stringList, ' ');
    // 255 255 255
    if(tokens == 3) {
        for(int i = 0; i < tokens; i++) {
            int r = CvHelper::stdStringToInt(stringList.at(0));
            int g = CvHelper::stdStringToInt(stringList.at(1));
            int b = CvHelper::stdStringToInt(stringList.at(2));
            cvScalarValue = cv::Scalar(r, g, b);
        }
    } else {
        throw "Value cannot convert to scalar object!";
    }

    return cvScalarValue;
}

template<> cv::Size SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);

    cv::Size cvSize;
    std::vector<std::string> stringList;

    int tokens = StringHelper::split(valueAsString, stringList, 'x');
    // 640x480
    if(tokens == 2) {
        int height = CvHelper::stdStringToInt(stringList.at(0));
        int width = CvHelper::stdStringToInt(stringList.at(1));
        cvSize = cv::Size(width, height);
    } else {
        throw "Value cannot convert to size object!";
    }

    return cvSize;
}

template<> cv::Size2f SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);

    cv::Size2f cvSizeF;
    std::vector<std::string> stringList;

    int tokens = StringHelper::split(valueAsString, stringList, 'x');
    // 640x480
    if(tokens == 2) {
        float height = CvHelper::stdStringToFloat(stringList.at(0));
        float width = CvHelper::stdStringToFloat(stringList.at(1));
        cvSizeF = cv::Size2f(width, height);
    } else {
        throw "Value cannot convert to size2f object!";
    }

    return cvSizeF;
}

template<> std::vector<cv::Point> SystemProperty::getValueOfParam(std::string paramName, int size) {
    std::string valueAsString = getValueOfParam<std::string>(paramName);
    QMutexLocker locker(&paramMutex);

    std::vector<cv::Point> pointList;
    std::vector<std::string> pointStringList;

    int tokens = StringHelper::split(valueAsString, pointStringList, ' ');
    // 151,454-152,26-562,32-572,446
    if(tokens == size) {
        std::vector<std::string> pstrings;
        for(int i = 0; i < tokens; i++) {
            int p = StringHelper::split(pointStringList.at(i), pstrings, ':');
            if(p != 2) {
                break;
            } else {
                int x = CvHelper::stdStringToInt(pstrings.at(0));
                int y = CvHelper::stdStringToInt(pstrings.at(1));
                pointList.push_back(cv::Point(x, y));
            }
        }
    }

    if(pointList.size() != size)
        return std::vector<cv::Point>();
    return pointList;
}

std::vector<Param> SystemProperty::getDefaultParamsFromQSettings() {
    QMutexLocker locker(&paramMutex);
    QSettings settings(QString::fromUtf8(CONFIGPARAM::CONFIG_INI_FILE.c_str()), QSettings::IniFormat);
    std::vector<Param> defaultParams;

    QString string;

    for(int key = 0; key < settings.allKeys().size(); key++) {
        string = settings.allKeys().at(key);

        std::string s = string.toUtf8().constData();
        std::string v = settings.value(string).toString().toUtf8().constData();
        defaultParams.push_back(Param(s, v));
    }
    return defaultParams;
}

