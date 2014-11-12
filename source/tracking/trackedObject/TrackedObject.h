#ifndef TrackedObject_h
#define TrackedObject_h

#include "ObjectModel.h"

#include <deque>
#include <memory>

#include <cereal/access.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>

class TrackedObject
{
public:
    TrackedObject(size_t id);
    //TODO: check if default ctor is really necessary for cereal
    TrackedObject() {}

    void add(const size_t framenumber, std::shared_ptr<ObjectModel> object);
    void push_back(std::shared_ptr<ObjectModel> object);

    size_t count(const size_t framenumber) const;

    std::shared_ptr<ObjectModel> get(const size_t framenumber) const;
    std::shared_ptr<ObjectModel> top() const;

    size_t id() const { return _id; }

private:
    int _id;
    std::map<size_t, std::shared_ptr<ObjectModel>> _objectsByFrame;

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(_id, _objectsByFrame);
    }
};


#endif // !TrackedObject_h

	
