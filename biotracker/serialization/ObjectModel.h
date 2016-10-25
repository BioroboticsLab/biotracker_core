#pragma once

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>

#include "biotracker/util/platform.h"

namespace BioTracker {
namespace Core {

/**
* class representing the object/animal
* which is to be tracked
*/
class BIOTRACKER_DLLEXPORT ObjectModel {
  public:
    virtual ~ObjectModel() = 0;
};

inline ObjectModel::~ObjectModel() {}

}
}
