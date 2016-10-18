#ifndef PSTATEWAITE_H
#define PSTATEWAITE_H

#include "Interfaces/IPlayerState.h"


class PStateWaite : public IPlayerState {
  public:
    PStateWaite(BioTracker3Player *player, IModel *textureObject,
                std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEWAITE_H
