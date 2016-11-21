#ifndef PSTATEWAIT_H
#define PSTATEWAIT_H

#include "Interfaces/IStates/IPlayerState.h"


class PStateWait : public IPlayerState {
  public:
    PStateWait(BioTracker3Player *player, std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEWAIT_H
