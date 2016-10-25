#include "PStateWait.h"
#include "Model/BioTracker3Player.h"


PStateWait::PStateWait(BioTracker3Player *player, IModel *textureObject,
                       std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStateWait::operate() {
    Q_EMIT emitOperationDone();
}
