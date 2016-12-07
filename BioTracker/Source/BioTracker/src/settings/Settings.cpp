#include "settings/Settings.h"

#include <boost/filesystem.hpp>

#include <QFile>
#include <QMessageBox>

#include "settings/Exceptions.h"
#include "settings/Messages.h"

namespace BioTracker {
namespace Core {

Settings::Settings() {
    if (!boost::filesystem::is_directory(ConfigParam::BASE_PATH)) {
        if (!boost::filesystem::create_directory(ConfigParam::BASE_PATH)) {
            throw BioTracker::Core::path_creation_error("Unable to create config dir");
        }
    }
    if (!boost::filesystem::is_directory(ConfigParam::MODULE_PATH)) {
        if (!boost::filesystem::create_directory(ConfigParam::MODULE_PATH)) {
            throw BioTracker::Core::path_creation_error("Unable to create module dir");
        }
    }
    if (!QFile::exists(QString::fromStdString(ConfigParam::CONFIGURATION_FILE.string()))) {
        QMessageBox::warning(nullptr, "No configuration file",
                             QString::fromStdString(Messages::System::MISSING_CONFIGURATION_FILE));
        _ptree = getDefaultParams();
        boost::property_tree::write_json(ConfigParam::CONFIGURATION_FILE.string(), _ptree);
    } else {
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ConfigParam::CONFIGURATION_FILE.string(), pt);
        _ptree = pt;
    }
}

const boost::property_tree::ptree Settings::getDefaultParams() {
    boost::property_tree::ptree pt;

    pt.put(TrackerParam::TRACKING_ENABLED, false);

    return pt;
}

}
}
