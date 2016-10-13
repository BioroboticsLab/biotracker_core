#ifndef PSTATEINITIAL_H
#define PSTATEINITIAL_H

#include "Interfaces/IPlayerState.h"

class PStateInitial : public IPlayerState {
  public:
    PStateInitial(BioTracker3Player *player, IModel *textureObject,
                  std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    void operate();

    bool stateOfPlay();
    bool stateOfRew();
    bool stateOfStepForward();
    bool stateOfStop();
};

#endif // PSTATEINITIAL_H
