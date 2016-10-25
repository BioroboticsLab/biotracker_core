#include "PStateInitial.h"
#include "Model/BioTracker3Player.h"

PStateInitial::PStateInitial(BioTracker3Player *player, IModel *textureObject,
                             std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {
    operate();
}

void PStateInitial::operate() {

    Q_EMIT emitStateOfPlay(false);
    Q_EMIT emitStateOfStepForward(false);
    Q_EMIT emitStateOfStepBackward(false);
    Q_EMIT emitStateOfStop(false);
    Q_EMIT emitStateOfPause(false);

    Q_EMIT emitNextState(IPlayerState::STATE_WAIT);

    Q_EMIT emitOperationDone();
}
