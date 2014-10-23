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
    void push_back(std::unique_ptr<ObjectModel>&& object);


private:
	int _id;	
    std::deque<std::unique_ptr<ObjectModel>> _objectTimeStamps;

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(_id, _objectTimeStamps);//, _objectTimeStamps);
    }
};


#endif // !TrackedObject_h

	
