#include "biotracker/serialization/BioTracker3TrackedObject.h"



#include <cassert>


BioTracker3TrackedObject::BioTracker3TrackedObject(size_t id) :
    _id(id),
    m_maximumFrameNumber(0) {
}

BioTracker3TrackedObject::BioTracker3TrackedObject() {}

void BioTracker3TrackedObject::add(const size_t framenumber,
                                   std::shared_ptr<IObject> object) {
    _objectsByFrame[framenumber] = object;
    if (framenumber > m_maximumFrameNumber) {
        m_maximumFrameNumber = framenumber;
    }
}

void BioTracker3TrackedObject::push_back(std::shared_ptr<IObject> object) {
    // get last idx with entry
    const size_t lastIdx = _objectsByFrame.rbegin() == _objectsByFrame.rend() ?
                           0 : _objectsByFrame.rbegin()->first;
    // add object with next idx
    _objectsByFrame.insert({lastIdx + 1, object});
}

size_t BioTracker3TrackedObject::count(const size_t framenumber) const {
    return _objectsByFrame.count(framenumber);
}

std::shared_ptr<IObject> BioTracker3TrackedObject::get(const size_t framenumber)
const {
    assert(_objectsByFrame.count(framenumber));
    return _objectsByFrame.at(framenumber);
}

std::shared_ptr<IObject> BioTracker3TrackedObject::top() const {
    assert(!_objectsByFrame.empty());
    return _objectsByFrame.rbegin()->second;
}

boost::optional<size_t> BioTracker3TrackedObject::getLastFrameNumber() const {
    if (_objectsByFrame.empty()) {
        return boost::optional<size_t>();
    } else {
        return _objectsByFrame.rbegin()->first;
    }
}

size_t BioTracker3TrackedObject::maximumFrameNumber() const {
    return m_maximumFrameNumber;
}
