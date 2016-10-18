#include "PStateWaite.h"

PStateWaite::PStateWaite(BioTracker3Player *player, IModel *textureObject,
                         std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStateWaite::operate() {
    Q_EMIT operationDone();
}
