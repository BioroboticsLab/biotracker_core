#ifndef TrackedObject_h
#define TrackedObject_h

#include "ObjectModel.h"

#include <deque>
#include <memory>

#include <cereal/access.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/deque.hpp>

class TrackedObject
{
public:
    TrackedObject(size_t id);
    //TODO: check if default ctor is really necessary
    TrackedObject() {}

    void push_back(std::shared_ptr<ObjectModel> object);
    std::shared_ptr<ObjectModel> top() const;

    size_t id() const { return _id; }

private:
	int _id;	
    std::deque<std::shared_ptr<ObjectModel>> _objectTimeStamps;

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(_id, _objectTimeStamps);
    }
};


#endif // !TrackedObject_h

	
