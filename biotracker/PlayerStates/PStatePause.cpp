#include "PStatePause.h"
#include "BioTracker3Player.h"

PStatePause::PStatePause(BioTracker3Player *player, IModel *textureObject,
                         std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStatePause::operate() {
//    bool xPlay = m_Player->getStateOfPlay();
//    bool xForw = m_Player->getStateOfStepForward();
//    bool xBack = m_Player->getStateOfStepBack();
//    bool xStop = m_Player->getStateOfStop();

//    Q_EMIT stateOfPlay(false);
//    Q_EMIT stateOfStepForward(false);
//    Q_EMIT stateOfStepBackward(false);
//    Q_EMIT stateOfStop(false);

//    m_Player-> setStateOfPlay(xPlay);
//    m_Player-> setStateOfStepForward(xForw);
//    m_Player-> setStateOfStepBackward(xBack);
//    m_Player-> setStateOfStop(xStop);

    Q_EMIT m_Player->notifyView();
}
