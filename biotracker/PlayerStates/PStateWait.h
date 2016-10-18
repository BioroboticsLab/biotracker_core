#ifndef PSTATEWAIT_H
#define PSTATEWAIT_H

#include "Interfaces/IPlayerState.h"


class PStateWait : public IPlayerState {
  public:
    PStateWait(BioTracker3Player *player, IModel *textureObject,
               std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEWAIT_H
