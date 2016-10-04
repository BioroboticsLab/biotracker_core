#ifndef INITIALSTATE_H
#define INITIALSTATE_H

#include "Interfaces/IPlayerState.h"

class InitialState : public IPlayerState {
  public:
    InitialState(BioTracker3Player *player, IModel *textureObject,
                 std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    void doIt();

    bool stateOfPlay();
    bool stateOfRew();
    bool stateOfStepForward();
    bool stateOfStop();
};

#endif // INITIALSTATE_H
