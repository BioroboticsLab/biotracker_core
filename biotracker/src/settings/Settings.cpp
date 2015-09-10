#include "settings/Settings.h"
#include "settings/Messages.h"

#include <QFile>
#include <QMessageBox>

Settings::Settings() {
    if (!QFile::exists(QString::fromStdString(CONFIGPARAM::CONFIGURATION_FILE))) {
        QMessageBox::warning(nullptr, "No configuration file",
                             QString::fromStdString(MSGS::SYSTEM::MISSING_CONFIGURATION_FILE));
        _ptree = getDefaultParams();
        boost::property_tree::write_json(CONFIGPARAM::CONFIGURATION_FILE, _ptree);
    } else {
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(CONFIGPARAM::CONFIGURATION_FILE, pt);
        _ptree = pt;
    }
}

const boost::property_tree::ptree Settings::getDefaultParams() {
    boost::property_tree::ptree pt;

    pt.put(TRACKERPARAM::TRACKING_ENABLED, false);

    return pt;
}
