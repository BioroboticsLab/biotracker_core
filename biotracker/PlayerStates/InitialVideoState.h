#ifndef INITIALVIDEOSTATE_H
#define INITIALVIDEOSTATE_H

#include "Interfaces/IPlayerState.h"

class InitialVideoState : public IPlayerState {
  public:
    InitialVideoState(BioTracker3Player *player, IModel *textureObject,
                      std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    void doIt();

    bool stateOfPlay();
    bool stateOfRew();
    bool stateOfStepForward();
    bool stateOfStop();

};

#endif // INITIALVIDEOSTATE_H
