#include "Registry.h"

#include <boost/filesystem/operations.hpp>

#include <QLibrary>

#include "source/core/Exceptions.h"

namespace BioTracker {
namespace Core {

Registry::Registry()
{
    _typeByString.insert(std::make_pair("No Tracking", NoTracking));
    _trackerByType.insert(std::make_pair(NoTracking, nullptr));
    _stringByType.insert(std::make_pair(NoTracking, "No Tracking"));
}

bool Registry::register_tracker_type(std::string name, new_tracker_function_t f)
{
    if (_typeByString.find(name) != _typeByString.end()) {
        throw std::invalid_argument("Tracker with same name already registered");
    }
    const TrackerType type = getNextId();
    _typeByString.emplace(name, type);
    _stringByType.emplace(type, name);
    _trackerByType.emplace(type, f);

    emit newTracker(type);

    return true;
}

void Registry::load_tracker_library(const boost::filesystem::path &path)
{
    typedef void (*RegisterFunction)();

    if (!boost::filesystem::exists(path)) {
        throw file_not_found("Could not find file " + path.string());
    }

    QLibrary trackerLibrary(QString::fromStdString(path.string()));
    auto registerFunction = static_cast<RegisterFunction>(trackerLibrary.resolve("registerTracker"));

    registerFunction();
}

std::shared_ptr<TrackingAlgorithm> Registry::make_new_tracker(const TrackerType type, Settings &settings, QWidget *parent) const
{
    const auto &it = _trackerByType.find(type);
    if (it != _trackerByType.cend()) {
        std::shared_ptr<TrackingAlgorithm> tracker = (it->second)(settings, parent);
        tracker->setType(type);
        return tracker;
    } else {
        return nullptr;
    }
}

TrackerType getNextId()
{
    static TrackerType nextType = NoTracking + 1;

    return nextType++;
}

}
}
