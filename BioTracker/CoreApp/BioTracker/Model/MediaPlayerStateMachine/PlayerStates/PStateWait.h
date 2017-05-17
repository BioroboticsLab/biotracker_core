#ifndef PSTATEWAIT_H
#define PSTATEWAIT_H

#include "IStates/IPlayerState.h"


class PStateWait : public IPlayerState {
  public:
    PStateWait(MediaPlayerStateMachine *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEWAIT_H
