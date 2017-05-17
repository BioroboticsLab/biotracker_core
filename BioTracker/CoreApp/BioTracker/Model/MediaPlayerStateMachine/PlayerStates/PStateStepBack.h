#ifndef PSTATESTEPBACK_H
#define PSTATESTEPBACK_H

#include "IStates/IPlayerState.h"


class PStateStepBack : public IPlayerState {
  public:
    PStateStepBack(MediaPlayerStateMachine *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATESTEPBACK_H
