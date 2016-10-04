#ifndef STATEVIDEOSTEPFORWARD_H
#define STATEVIDEOSTEPFORWARD_H

#include "Interfaces/IPlayerState.h"

class StateVideoStepForward : public IPlayerState {
  public:
    StateVideoStepForward(BioTracker3Player *player, IModel *textureObject,
                          std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    void doIt();

    bool stateOfPlay();
    bool stateOfRew();
    bool stateOfStepForward();
    bool stateOfStop();
};

#endif // STATEVIDEOSTEPFORWARD_H
