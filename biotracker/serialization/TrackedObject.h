#ifndef TrackedObject_h
#define TrackedObject_h

#include "ObjectModel.h"

#include <deque>
#include <memory>

#include <boost/optional.hpp>

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>

#include "types.hpp"

class TrackedObject {
  public:
    TrackedObject(size_t id);
    //TODO: check if default ctor is really necessary for cereal
    TrackedObject() {}

    void add(const size_t framenumber, std::shared_ptr<ObjectModel> object);
    void push_back(std::shared_ptr<ObjectModel> object);

    size_t count(const size_t framenumber) const;

    std::shared_ptr<ObjectModel> get(const size_t framenumber) const;

    template<typename DerivedObjectModel>
    std::shared_ptr<DerivedObjectModel> get(const size_t framenumber) const {
        std::shared_ptr<ObjectModel> object = get(framenumber);
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

    std::shared_ptr<ObjectModel> top() const;

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

  private:
    size_t _id;
    std::map<size_t, std::shared_ptr<ObjectModel>> _objectsByFrame;

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(_id), CEREAL_NVP(_objectsByFrame));
    }
};

#endif // !TrackedObject_h


