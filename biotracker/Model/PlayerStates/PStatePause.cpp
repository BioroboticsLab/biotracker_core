#include "PStatePause.h"
#include "Model/BioTracker3Player.h"

PStatePause::PStatePause(BioTracker3Player *player, IModel *textureObject,
                         std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStatePause::operate() {
//    bool xPlay = m_Player->getStateOfPlay();
//    bool xForw = m_Player->getStateOfStepForward();
//    bool xBack = m_Player->getStateOfStepBack();
//    bool xStop = m_Player->getStateOfStop();

    Q_EMIT emitStateOfPlay(true);
    Q_EMIT emitStateOfStepForward(true);
    Q_EMIT emitStateOfStepBackward(true);
    Q_EMIT emitStateOfStop(true);
    Q_EMIT emitStateOfPause(false);

    Q_EMIT emitOperationDone();
}
