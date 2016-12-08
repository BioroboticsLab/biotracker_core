#ifndef PSTATEINITIALSTREAM_H
#define PSTATEINITIALSTREAM_H

#include "IStates/IPlayerState.h"

class PStateInitialStream : public IPlayerState {
  public:
    PStateInitialStream(MediaPlayer *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // INITIALVIDEOSTATE_H
