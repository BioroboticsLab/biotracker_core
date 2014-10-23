#include "TrackedObject.h"

TrackedObject::TrackedObject(size_t id)
    : _id(id)
{
    _objectTimeStamps.push_back(std::unique_ptr<ObjectModel>(new DerivedObjectModel(1)));
    _objectTimeStamps.push_back(std::unique_ptr<ObjectModel>(new DerivedObjectModel(2)));
}

void TrackedObject::push_back(std::unique_ptr<ObjectModel>&& object)
{
    _objectTimeStamps.push_back(std::move(object));
}
