#ifndef PSTATESTEPBACK_H
#define PSTATESTEPBACK_H

#include "Interfaces/IPlayerState.h"


class PStateStepBack : public IPlayerState {
  public:
    PStateStepBack(BioTracker3Player *player, IModel *textureObject,
                   std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATESTEPBACK_H
