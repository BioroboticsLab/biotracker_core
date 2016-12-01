#include "BioTracker3Player.h"

#include "PlayerStates/PStatePlay.h"
#include "PlayerStates/PStateInitialStream.h"
#include "PlayerStates/PStateStepForw.h"
#include "PlayerStates/PStateInitial.h"
#include "PlayerStates/PStatePause.h"
#include "PlayerStates/PStateStepBack.h"
#include "PlayerStates/PStateWait.h"

BioTracker3Player::BioTracker3Player(QObject *parent) :
    IModel(parent),
    m_ImageStream(BioTracker::Core::make_ImageStream3NoMedia()) {

    m_States.insert(IPlayerState::PLAYER_STATES::STATE_INITIAL, (new PStateInitial(this, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_INITIAL_STREAM, (new PStateInitialStream(this, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_STEP_FORW, (new PStateStepForw(this, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_PLAY, (new PStatePlay(this, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_PAUSE, (new PStatePause(this,m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_STEP_BACK, (new PStateStepBack(this, m_ImageStream)));
    m_States.insert(IPlayerState::PLAYER_STATES::STATE_WAIT, (new PStateWait(this, m_ImageStream)));

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *>::iterator i;
    for (i = m_States.begin(); i != m_States.end(); ++i)
        QObject::connect(i.value(), &IPlayerState::emitStateDone, this, &BioTracker3Player::receiveStateDone);

    setNextState(IPlayerState::PLAYER_STATES::STATE_INITIAL);
}

void BioTracker3Player::runPlayerOperation() {

    if(m_NextPlayerState != m_States.value(IPlayerState::PLAYER_STATES::STATE_WAIT)) {

        m_CurrentPlayerState = m_NextPlayerState;

        m_CurrentPlayerState->operate();
        updatePlayerParameter();
        emitSignals();

    }

}

void BioTracker3Player::receiveLoadVideoCommand(QString fileDir)
{
    std::string filenameStr = fileDir.toStdString();

    boost::filesystem::path filename {filenameStr};

    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> stream(BioTracker::Core::make_ImageStream3Video(filename));

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *>::iterator i;
    for (i = m_States.begin(); i != m_States.end(); i++) {
        i.value()->changeImageStream(stream);
    }

    setNextState(IPlayerState::STATE_INITIAL_STREAM);

}

void BioTracker3Player::receiveLoadPictures(std::vector<boost::filesystem::path> files)
{
    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> stream(BioTracker::Core::make_ImageStream3Pictures(files));

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *>::iterator i;
    for (i = m_States.begin(); i != m_States.end(); i++) {
        i.value()->changeImageStream(stream);
    }

    setNextState(IPlayerState::STATE_INITIAL_STREAM);

}

void BioTracker3Player::receiveLoadCameraDevice(int x)
{
    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> stream(BioTracker::Core::make_ImageStream3Camera(x));

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *>::iterator i;
    for (i = m_States.begin(); i != m_States.end(); i++) {
        i.value()->changeImageStream(stream);
    }

    setNextState(IPlayerState::STATE_INITIAL_STREAM);
}

void BioTracker3Player::receivePrevFrameCommand()
{
    setNextState(IPlayerState::STATE_STEP_BACK);
}

void BioTracker3Player::receiveNextFramCommand()
{
    setNextState(IPlayerState::STATE_STEP_FORW);
}

void BioTracker3Player::receivePauseCommand()
{
    setNextState(IPlayerState::STATE_PAUSE);
}

void BioTracker3Player::receiveStopCommand()
{
    setNextState(IPlayerState::STATE_INITIAL_STREAM);
}

void BioTracker3Player::receivePlayCommand()
{
    setNextState(IPlayerState::STATE_PLAY);
}

void BioTracker3Player::receiveStateDone()
{
}


void BioTracker3Player::updatePlayerParameter()
{
    m_MediaType = m_ImageStream->type();
    m_TotalNumbFrames = m_ImageStream->numFrames();
    m_fps = m_ImageStream->fps();
    m_CurrentFilename = QString::fromStdString( m_ImageStream->currentFilename() );

    m_Back = m_CurrentPlayerState->getStateForBackward();
    m_Forw = m_CurrentPlayerState->getStateForForward();
    m_Paus = m_CurrentPlayerState->getStateForPause();
    m_Play = m_CurrentPlayerState->getStateForPlay();
    m_Stop = m_CurrentPlayerState->getStateForStop();
    m_CurrentFrame = m_CurrentPlayerState->getCurrentFrame();
    m_CurrentFrameNumber = m_CurrentPlayerState->getCurrentFrameNumber();

    m_VideoControllsStates.clear();
    m_VideoControllsStates.append(m_Back);
    m_VideoControllsStates.append(m_Forw);
    m_VideoControllsStates.append(m_Paus);
    m_VideoControllsStates.append(m_Play);
    m_VideoControllsStates.append(m_Stop);

}

void BioTracker3Player::emitSignals()
{

    Q_EMIT emitVideoControllsStates(m_VideoControllsStates);
    Q_EMIT emitMediaType(m_MediaType);
    Q_EMIT emitTotalNumbFrames(m_TotalNumbFrames);
    Q_EMIT emitCurrentFileName(m_CurrentFilename);
    Q_EMIT emitCurrentFrameStr(m_CurrentFrame, m_NameOfCvMat);
    Q_EMIT emitCurrentFrameNumber(m_CurrentFrameNumber);
    Q_EMIT emitFPS(m_fps);

    Q_EMIT emitCurrentFrame(m_CurrentFrame);

    Q_EMIT notifyView();
}

void BioTracker3Player::setNextState(IPlayerState::PLAYER_STATES state) {
    m_NextPlayerState = m_States.value(state);

    Q_EMIT emitPlayerOperationDone();

}


