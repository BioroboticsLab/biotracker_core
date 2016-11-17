#ifndef BIOTRACKER3TRACKEDOBJECT_H
#define BIOTRACKER3TRACKEDOBJECT_H

#include <deque>
#include <memory>

#include <boost/optional.hpp>

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>

#include "biotracker/util/platform.h"
#include "types.hpp"

#include "Interfaces/IModel/IObject.h"

#include "Interfaces/IModel/imodel.h"


class BIOTRACKER_DLLEXPORT BioTracker3TrackedObject
{

  public:
    BioTracker3TrackedObject(size_t id);
    //TODO: check if default ctor is really necessary for cereal
    BioTracker3TrackedObject();

    void add(const size_t framenumber, std::shared_ptr<IObject> object);
    void push_back(std::shared_ptr<IObject> object);

    size_t count(const size_t framenumber) const;

    std::shared_ptr<IObject> get(const size_t framenumber) const;

    template<typename DerivedObjectModel>
    std::shared_ptr<DerivedObjectModel> get(const size_t framenumber) const {
        std::shared_ptr<IObject> object = get(framenumber);
        return std::dynamic_pointer_cast<DerivedObjectModel>(object);
    }

    template<typename DerivedObjectModel>
    std::shared_ptr<DerivedObjectModel> maybeGet(const size_t framenumber) const {
        auto it = _objectsByFrame.find(framenumber);

        if (it == _objectsByFrame.end()) {
            return std::shared_ptr<DerivedObjectModel>();
        } else {
            return std::dynamic_pointer_cast<DerivedObjectModel>(it->second);
        }
    }

    std::shared_ptr<IObject> top() const;

    size_t getId() const {
        return _id;
    }
    void setId(size_t id) {
        _id = id;
    }

    bool isEmpty() const {
        return _objectsByFrame.empty();
    }
    void erase(size_t framenumber) {
        _objectsByFrame.erase(framenumber);
    }
    boost::optional<size_t> getLastFrameNumber() const;

    bool hasValuesAtFrame(const size_t frameNumber) {
        return _objectsByFrame.find(frameNumber) != _objectsByFrame.end();
    }

    size_t maximumFrameNumber() const;

  private:
    size_t _id;
    size_t m_maximumFrameNumber;

#ifdef _MSC_VER
    // Disable DLL export warning for this member. This is only possible
    // because the member is private and can not be accessed directly when using the DLL anyway.
#  pragma warning( push )
#  pragma warning( disable: 4251 )
#endif
    std::map<size_t, std::shared_ptr<IObject>> _objectsByFrame ;
#ifdef _MSC_VER
#  pragma warning( pop )
#endif
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(_id), CEREAL_NVP(_objectsByFrame));
    }
};



#endif // BIOTRACKER3TRACKEDOBJECT_H
