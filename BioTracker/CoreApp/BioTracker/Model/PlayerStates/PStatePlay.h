#ifndef PSTATEPLAY_H
#define PSTATEPLAY_H

#include "IStates/IPlayerState.h"


class PStatePlay : public IPlayerState {
  public:
    PStatePlay(MediaPlayer *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEPLAY_H
