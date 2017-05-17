#ifndef PSTATEINITIAL_H
#define PSTATEINITIAL_H

#include "IStates/IPlayerState.h"

class PStateInitial : public IPlayerState {
  public:
    PStateInitial(MediaPlayerStateMachine *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEINITIAL_H
