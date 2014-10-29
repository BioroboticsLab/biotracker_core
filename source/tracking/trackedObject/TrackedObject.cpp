#include "TrackedObject.h"

#include <cassert>

TrackedObject::TrackedObject(size_t id)
    : _id(id)
{
}

void TrackedObject::push_back(std::shared_ptr<ObjectModel> object)
{
    _objectTimeStamps.push_back(object);
}

std::shared_ptr<ObjectModel> TrackedObject::top() const
{
    assert(_objectTimeStamps.size());
    return _objectTimeStamps.at(_objectTimeStamps.size());
}
