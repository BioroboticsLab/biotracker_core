#include "algorithms.h"

namespace Algorithms {

Registry::Registry()
{
	_typeByString.insert(std::make_pair("No Tracking", NoTracking));
	_trackerByType.insert(std::make_pair(NoTracking, nullptr));
}

bool Registry::register_tracker_type(std::string name, new_tracker_function_t f)
{
    if (_typeByString.find(name) != _typeByString.end()) {
        throw std::invalid_argument("Tracker with same name already registered");
    }
    const Type type = getNextId();
    _typeByString.emplace(std::move(name), type);
    _trackerByType.emplace(type, f);

    return true;
}

std::shared_ptr<TrackingAlgorithm> Registry::make_new_tracker(const Type type, Settings &settings, std::string &serializationPath, QWidget *parent) const
{
    const auto &it = _trackerByType.find(type);
    if (it != _trackerByType.cend()) {
        return (it->second)(settings, serializationPath, parent);
    } else {
        return nullptr;
    }
}

Type getNextId()
{
    static Type nextType = NoTracking + 1;

    return nextType++;
}

}
