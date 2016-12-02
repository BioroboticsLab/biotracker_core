#ifndef PSTATEPLAY_H
#define PSTATEPLAY_H

#include "IStates/IPlayerState.h"


class PStatePlay : public IPlayerState {
  public:
    PStatePlay(BioTracker3Player *player, std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEPLAY_H
