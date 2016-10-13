#ifndef PSTATEPLAY_H
#define PSTATEPLAY_H

#include "Interfaces/IPlayerState.h"


class PStatePlay : public IPlayerState {
  public:
    PStatePlay(BioTracker3Player *player, IModel *textureObject,
               std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    void operate();

    bool stateOfPlay();
    bool stateOfRew();
    bool stateOfStepForward();
    bool stateOfStop();

};

#endif // PSTATEPLAY_H
