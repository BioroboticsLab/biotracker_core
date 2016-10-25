#include "BioTracker3Player.h"

#include "PlayerStates/PStatePlay.h"
#include "PlayerStates/PStateInitialStream.h"
#include "PlayerStates/PStateStepForw.h"
#include "PlayerStates/PStateInitial.h"
#include "PlayerStates/PStatePause.h"
#include "PlayerStates/PStateStepBack.h"
#include "PlayerStates/PStateWait.h"

BioTracker3Player::BioTracker3Player(BioTracker::Core::BioTracker3TextureObject *textureObject) :
    m_TextureObjectModel(textureObject),
    m_ImageStream(BioTracker::Core::make_ImageStream3NoMedia()) {

    m_States.insert(IPlayerState::PLAYER_STATES::STATE_INITIAL, (new PStateInitial(this, m_TextureObjectModel,
                    m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_INITIAL_STREAM, (new PStateInitialStream(this, m_TextureObjectModel,
                    m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_STEP_FORW, (new PStateStepForw(this,
                    m_TextureObjectModel, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_PLAY, (new PStatePlay(this, m_TextureObjectModel, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_PAUSE, (new PStatePause(this, m_TextureObjectModel, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_STEP_BACK, (new PStateStepBack(this, m_TextureObjectModel,
                    m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_WAIT, (new PStateWait(this, m_TextureObjectModel, m_ImageStream)));

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *>::iterator i;
    for (i = m_States.begin(); i != m_States.end(); i++) {
        QObject::connect(i.value(), &IPlayerState::emitNextState, this, &BioTracker3Player::setNextState);
        QObject::connect(this, &BioTracker3Player::emitChangeImageStream, i.value(), &IPlayerState::changeImageStream);
        QObject::connect(this, &BioTracker3Player::operate, i.value(), &IPlayerState::operate, Qt::BlockingQueuedConnection);
        QObject::connect(i.value(), &IPlayerState::emitStateOfPlay, this, &BioTracker3Player::setStateOfPlay);
        QObject::connect(i.value(), &IPlayerState::emitStateOfStepBackward, this, &BioTracker3Player::setStateOfStepBackward);
        QObject::connect(i.value(), &IPlayerState::emitStateOfStepForward, this, &BioTracker3Player::setStateOfStepForward);
        QObject::connect(i.value(), &IPlayerState::emitStateOfStop, this, &BioTracker3Player::setStateOfStop);
        QObject::connect(i.value(), &IPlayerState::emitStateOfPause, this, &BioTracker3Player::setStateOfPause);
        QObject::connect(i.value(), &IPlayerState::emitOperationDone, this, &BioTracker3Player::handelStateDone);

        //i.value()->moveToThread(&m_StateThread);
    }

//    m_ImageStream->moveToThread(&m_StateThread);
    // m_StateThread.start();

    setCurrentState(IPlayerState::STATE_INITIAL);
    m_CurrentPlayerState = m_States.value(IPlayerState::PLAYER_STATES::STATE_INITIAL);
}

void BioTracker3Player::operate() {
    Q_EMIT m_CurrentPlayerState->operate();
}

void BioTracker3Player::loadVideo(QString str) {
    std::string filenameStr = str.toStdString();

    boost::filesystem::path filename {filenameStr};

    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> stream(BioTracker::Core::make_ImageStream3Video(filename));

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *>::iterator i;
    for (i = m_States.begin(); i != m_States.end(); i++) {
        i.value()->changeImageStream(stream);
    }
    setCurrentState(IPlayerState::STATE_INITIAL_STREAM);
}

void BioTracker3Player::nextFrame() {
    setCurrentState(IPlayerState::STATE_STEP_FORW);
}

void BioTracker3Player::prevFrame() {
    setCurrentState(IPlayerState::STATE_STEP_BACK);
}

void BioTracker3Player::play() {
    setCurrentState(IPlayerState::STATE_PLAY);
}

void BioTracker3Player::stop() {
    setCurrentState(IPlayerState::STATE_INITIAL_STREAM);
}

void BioTracker3Player::pause() {
    setCurrentState(IPlayerState::STATE_PAUSE);
}

void BioTracker3Player::setCurrentState(IPlayerState::PLAYER_STATES state) {
    m_CurrentPlayerState = m_States.value(state);

    Q_EMIT notifyView();
}

void BioTracker3Player::handelStateDone() {
    Q_EMIT notifyView();
    Q_EMIT emitStateChangeDone();
}

void BioTracker3Player::setStateOfStepForward(bool xState) {
    m_StateOfStepForward = xState;
}

void BioTracker3Player::setStateOfStepBackward(bool xState) {
    m_StateOfStepBack = xState;
}

void BioTracker3Player::setStateOfPlay(bool xState) {
    m_StateOfPlay = xState;
}

void BioTracker3Player::setStateOfStop(bool xState) {
    m_StateOfStop = xState;
}

void BioTracker3Player::setStateOfPause(bool xState) {
    m_StateOfPause = xState;
}

bool BioTracker3Player::getStateOfStepForward() {
    return m_StateOfStepForward;
}

bool BioTracker3Player::getStateOfPlay() {
    return m_StateOfPlay;
}

bool BioTracker3Player::getStateOfStepBack() {
    return m_StateOfStepBack;
}

bool BioTracker3Player::getStateOfStop() {
    return m_StateOfStop;
}

bool BioTracker3Player::getStateOfPause() {
    return m_StateOfPause;
}

IModel *BioTracker3Player::getStateModel() {
    return dynamic_cast<IModel *>(m_CurrentPlayerState);
}

void BioTracker3Player::setNextState(IPlayerState::PLAYER_STATES state) {
    setCurrentState(state);
}
