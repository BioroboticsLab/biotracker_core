#ifndef PLAYING_H
#define PLAYING_H

#include "Interfaces/IPlayerState.h"


class Playing : public IPlayerState {
  public:
    Playing(BioTracker3Player *player, IModel *textureObject,
            std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    void doIt();

    bool stateOfPlay();
    bool stateOfRew();
    bool stateOfStepForward();
    bool stateOfStop();

};

#endif // PLAYING_H
