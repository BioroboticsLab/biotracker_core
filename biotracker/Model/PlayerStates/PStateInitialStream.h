#ifndef PSTATEINITIALSTREAM_H
#define PSTATEINITIALSTREAM_H

#include "Interfaces/IPlayerState.h"

class PStateInitialStream : public IPlayerState {
  public:
    PStateInitialStream(BioTracker3Player *player, std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // INITIALVIDEOSTATE_H
