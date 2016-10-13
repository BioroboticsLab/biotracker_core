#ifndef PSTATEINITIALSTREAM_H
#define PSTATEINITIALSTREAM_H

#include "Interfaces/IPlayerState.h"

class PStateInitialStream : public IPlayerState {
  public:
    PStateInitialStream(BioTracker3Player *player, IModel *textureObject,
                        std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    void operate();

    bool stateOfPlay();
    bool stateOfRew();
    bool stateOfStepForward();
    bool stateOfStop();

};

#endif // INITIALVIDEOSTATE_H
