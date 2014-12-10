#include "algorithms.h"

namespace Algorithms {

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
	const Type type = getNextId();
	_typeByString.emplace(name, type);
	_stringByType.emplace(type, name);
	_trackerByType.emplace(type, f);

	return true;
}

std::shared_ptr<TrackingAlgorithm> Registry::make_new_tracker(const Type type, Settings &settings, QWidget *parent) const
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

Type getNextId()
{
	static Type nextType = NoTracking + 1;

	return nextType++;
}

}
