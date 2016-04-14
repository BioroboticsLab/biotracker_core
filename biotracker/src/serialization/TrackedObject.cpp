#include "serialization/TrackedObject.h"

#include <cassert>

namespace BioTracker {
namespace Core {

TrackedObject::TrackedObject(size_t id)
    : _id(id),
      m_maximumFrameNumber(0) {
}

void TrackedObject::add(const size_t framenumber,
                        std::shared_ptr<ObjectModel> object) {
    _objectsByFrame[framenumber] = object;
    if (framenumber > m_maximumFrameNumber) {
        m_maximumFrameNumber = framenumber;
    }
}

void TrackedObject::push_back(std::shared_ptr<ObjectModel> object) {
    // get last idx with entry
    const size_t lastIdx = _objectsByFrame.rbegin() == _objectsByFrame.rend() ?
                           0 : _objectsByFrame.rbegin()->first;
    // add object with next idx
    _objectsByFrame.insert({lastIdx + 1, object});
}

size_t TrackedObject::count(const size_t framenumber) const {
    return _objectsByFrame.count(framenumber);
}

std::shared_ptr<ObjectModel> TrackedObject::get(const size_t framenumber)
const {
    assert(_objectsByFrame.count(framenumber));
    return _objectsByFrame.at(framenumber);
}

std::shared_ptr<ObjectModel> TrackedObject::top() const {
    assert(!_objectsByFrame.empty());
    return _objectsByFrame.rbegin()->second;
}

boost::optional<size_t> TrackedObject::getLastFrameNumber() const {
    if (_objectsByFrame.empty()) {
        return boost::optional<size_t>();
    } else {
        return _objectsByFrame.rbegin()->first;
    }
}

size_t TrackedObject::maximumFrameNumber() const {
    return m_maximumFrameNumber;
}

}
}

