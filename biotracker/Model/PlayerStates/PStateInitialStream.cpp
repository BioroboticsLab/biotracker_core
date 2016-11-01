#include "PStateInitialStream.h"
#include "Model/BioTracker3Player.h"

PStateInitialStream::PStateInitialStream(BioTracker3Player *player, IModel *textureObject,
        std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStateInitialStream::operate() {

    bool xval = false;
    IPlayerState::PLAYER_STATES state = IPlayerState::STATE_INITIAL;
    xval = m_ImageStream->setFrameNumber(0);

    if (xval) {
        cv::Mat mat = m_ImageStream->currentFrame();
        dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(mat);
    }


    Q_EMIT emitStateOfPlay(true);
    Q_EMIT emitStateOfStepForward(true);
    Q_EMIT emitStateOfStepBackward(false);
    Q_EMIT emitStateOfStop(false);
    Q_EMIT emitStateOfPause(false);


    Q_EMIT emitNextState(IPlayerState::STATE_WAIT);

    Q_EMIT emitOperationDone();
}
