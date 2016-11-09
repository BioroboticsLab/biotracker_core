#ifndef PSTATEPAUSE_H
#define PSTATEPAUSE_H

#include "Interfaces/IPlayerState.h"


class PStatePause : public IPlayerState {
  public:
    PStatePause(BioTracker3Player *player, std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;

};

#endif // PSTATEPAUSE_H
