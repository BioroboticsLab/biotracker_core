#ifndef PSTATEPAUSE_H
#define PSTATEPAUSE_H

#include "IStates/IPlayerState.h"


class PStatePause : public IPlayerState {
  public:
    PStatePause(MediaPlayerStateMachine *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;

};

#endif // PSTATEPAUSE_H
