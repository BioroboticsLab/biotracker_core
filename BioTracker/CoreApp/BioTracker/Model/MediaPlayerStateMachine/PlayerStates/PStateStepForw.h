#ifndef PSTATESTEPFORW_H
#define PSTATESTEPFORW_H

#include "IStates/IPlayerState.h"

class PStateStepForw : public IPlayerState {
  public:
    PStateStepForw(MediaPlayerStateMachine *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATESTEPFORW_H
