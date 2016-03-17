#pragma once

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>

namespace BioTracker {
namespace Core {

/**
* class representing the object/animal
* which is to be tracked
*/
class ObjectModel {
  public:
    virtual ~ObjectModel() = 0;
};

inline ObjectModel::~ObjectModel() {}

}
}
