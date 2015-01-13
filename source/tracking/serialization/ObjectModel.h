#ifndef ObjectModel_H
#define ObjectModel_H

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>

/**
* class representing the object/animal 
* which is to be tracked
*/
class ObjectModel 
{
public:
    virtual ~ObjectModel() = 0;
};

inline ObjectModel::~ObjectModel() {}

#endif // !ObjectModel_H

