#include "BioTracker3Player.h"

#include "PlayerStates/Playing.h"
#include "PlayerStates/InitialState.h"
#include "PlayerStates/InitialVideoState.h"
#include "PlayerStates/StateVideoStepForward.h"

BioTracker3Player::BioTracker3Player(BioTracker::Core::BioTracker3TextureObject *textureObject) :
    m_TextureObjectModel(textureObject),
    m_ImageStream(BioTracker::Core::make_ImageStream3NoMedia()) {

    m_States.insert(IPlayerState::PLAYER_STATES::STATE_INITIAL, (new InitialState(this, m_TextureObjectModel,
                    m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_VIDEO_INITIAL, (new InitialVideoState(this, m_TextureObjectModel,
                    m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_VIDEO_STEPNEXT, (new StateVideoStepForward(this,
                    m_TextureObjectModel, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_PLAY, (new Playing(this, m_TextureObjectModel, m_ImageStream)));

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *>::iterator i;
    for (i = m_States.begin(); i != m_States.end(); i++) {
        QObject::connect(i.value(), &IPlayerState::emitNextState, this, &BioTracker3Player::setNextState);
    }
    m_CurrentPlayerState = m_States.value(IPlayerState::PLAYER_STATES::STATE_INITIAL);
}

void BioTracker3Player::loadVideo(QString str) {
    std::string filenameStr = str.toStdString();

    boost::filesystem::path filename {filenameStr};

    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> stream(BioTracker::Core::make_ImageStream3Video(filename));

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *>::iterator i;
    for (i = m_States.begin(); i != m_States.end(); i++) {
        i.value()->changeImageStream(stream);
    }
    setCurrentState(IPlayerState::STATE_VIDEO_INITIAL);
}

void BioTracker3Player::nextFrame() {
    setCurrentState(IPlayerState::STATE_VIDEO_STEPNEXT);
}

void BioTracker3Player::play() {
    setCurrentState(IPlayerState::STATE_PLAY);
}

void BioTracker3Player::setCurrentState(IPlayerState::PLAYER_STATES state) {
    m_CurrentPlayerState = m_States.value(state);

    Q_EMIT m_CurrentPlayerState->doIt();
}

bool BioTracker3Player::getStateOfForward() {
    return m_CurrentPlayerState->stateOfStepForward();
}

bool BioTracker3Player::getStateOfPlay() {
    return m_CurrentPlayerState->stateOfPlay();
}

bool BioTracker3Player::getStateOfRew() {
    return m_CurrentPlayerState->stateOfRew();
}

bool BioTracker3Player::getStateOfStop() {
    return m_CurrentPlayerState->stateOfStop();
}

IModel *BioTracker3Player::getStateModel() {
    return dynamic_cast<IModel *>(m_CurrentPlayerState);
}

void BioTracker3Player::setNextState(IPlayerState::PLAYER_STATES state) {
    setCurrentState(state);
}
