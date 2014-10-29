#ifndef ObjectModel_H
#define ObjectModel_H

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

class DerivedObjectModel : public ObjectModel
{
public:
    DerivedObjectModel() : x(0) {};
    DerivedObjectModel(int x) : x(x) {};
private:
    int x;

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(x);
    }
};


#endif // !ObjectModel_H

