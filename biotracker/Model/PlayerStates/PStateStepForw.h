#ifndef PSTATESTEPFORW_H
#define PSTATESTEPFORW_H

#include "Interfaces/IPlayerState.h"

class PStateStepForw : public IPlayerState {
  public:
    PStateStepForw(BioTracker3Player *player, IModel *textureObject,
                   std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATESTEPFORW_H
