#ifndef PSTATEINITIAL_H
#define PSTATEINITIAL_H

#include "Interfaces/IPlayerState.h"

class PStateInitial : public IPlayerState {
  public:
    PStateInitial(BioTracker3Player *player, std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEINITIAL_H
