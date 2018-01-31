#include "MediaPlayerStateMachine.h"

#include "PlayerStates/PStatePlay.h"
#include "PlayerStates/PStateInitialStream.h"
#include "PlayerStates/PStateStepForw.h"
#include "PlayerStates/PStateInitial.h"
#include "PlayerStates/PStatePause.h"
#include "PlayerStates/PStateStepBack.h"
#include "PlayerStates/PStateWait.h"
#include "PlayerStates/PStateGoToFrame.h"

#include "util/types.h"

MediaPlayerStateMachine::MediaPlayerStateMachine(QObject* parent) :
	IModel(parent),
	m_ImageStream(BioTracker::Core::make_ImageStream3NoMedia()) {

	m_PlayerParameters = new playerParameters();

	m_States.insert(IPlayerState::PLAYER_STATES::STATE_INITIAL, (new PStateInitial(this, m_ImageStream)));
	m_States.insert(IPlayerState::PLAYER_STATES::STATE_INITIAL_STREAM, (new PStateInitialStream(this, m_ImageStream)));
	m_States.insert(IPlayerState::PLAYER_STATES::STATE_STEP_FORW, (new PStateStepForw(this, m_ImageStream)));
	m_States.insert(IPlayerState::PLAYER_STATES::STATE_PLAY, (new PStatePlay(this, m_ImageStream)));
	m_States.insert(IPlayerState::PLAYER_STATES::STATE_PAUSE, (new PStatePause(this, m_ImageStream)));
	m_States.insert(IPlayerState::PLAYER_STATES::STATE_STEP_BACK, (new PStateStepBack(this, m_ImageStream)));
	m_States.insert(IPlayerState::PLAYER_STATES::STATE_WAIT, (new PStateWait(this, m_ImageStream)));
	m_States.insert(IPlayerState::PLAYER_STATES::STATE_GOTOFRAME, (new PStateGoToFrame(this, m_ImageStream)));

	setNextState(IPlayerState::PLAYER_STATES::STATE_INITIAL);
}

void MediaPlayerStateMachine::receiveRunPlayerOperation() {

	if (m_NextPlayerState != m_States.value(IPlayerState::PLAYER_STATES::STATE_WAIT)) {

		m_CurrentPlayerState = m_NextPlayerState;

		m_CurrentPlayerState->operate();
		updatePlayerParameter();
		emitSignals();

	}

}

void MediaPlayerStateMachine::receiveLoadVideoCommand(QString fileDir) {
	std::string filenameStr = fileDir.toStdString();

	boost::filesystem::path filename{ filenameStr };

	m_stream = BioTracker::Core::make_ImageStream3Video(filename);

	m_PlayerParameters->m_TotalNumbFrames = m_stream->numFrames();

	QMap<IPlayerState::PLAYER_STATES, IPlayerState*>::iterator i;
	for (i = m_States.begin(); i != m_States.end(); i++) {
		i.value()->changeImageStream(m_stream);
	}

	setNextState(IPlayerState::STATE_INITIAL_STREAM);

}

void MediaPlayerStateMachine::receiveLoadPictures(std::vector<boost::filesystem::path> files) {
	m_stream = BioTracker::Core::make_ImageStream3Pictures(files);

	m_PlayerParameters->m_TotalNumbFrames = m_stream->numFrames();

	QMap<IPlayerState::PLAYER_STATES, IPlayerState*>::iterator i;
	for (i = m_States.begin(); i != m_States.end(); i++) {
		i.value()->changeImageStream(m_stream);
	}

	setNextState(IPlayerState::STATE_INITIAL_STREAM);

}

void MediaPlayerStateMachine::receiveLoadCameraDevice(CameraConfiguration conf) {
	m_stream = BioTracker::Core::make_ImageStream3Camera(conf);

	m_PlayerParameters->m_TotalNumbFrames = m_stream->numFrames();

	QMap<IPlayerState::PLAYER_STATES, IPlayerState*>::iterator i;
	for (i = m_States.begin(); i != m_States.end(); i++) {
		i.value()->changeImageStream(m_stream);
	}

	setNextState(IPlayerState::STATE_INITIAL_STREAM);
}

void MediaPlayerStateMachine::receivePrevFrameCommand() {
	setNextState(IPlayerState::STATE_STEP_BACK);
}

void MediaPlayerStateMachine::receiveNextFramCommand() {
	setNextState(IPlayerState::STATE_STEP_FORW);
}

void MediaPlayerStateMachine::receivePauseCommand() {
	setNextState(IPlayerState::STATE_PAUSE);
}

void MediaPlayerStateMachine::receiveStopCommand() {
	setNextState(IPlayerState::STATE_INITIAL_STREAM);
}

void MediaPlayerStateMachine::receivePlayCommand() {
	setNextState(IPlayerState::STATE_PLAY);
}

void MediaPlayerStateMachine::receiveGoToFrame(int frame) {
	PStateGoToFrame* state = dynamic_cast<PStateGoToFrame*> (m_States.value(IPlayerState::PLAYER_STATES::STATE_GOTOFRAME));
	state->setFrameNumber(frame);
	setNextState(IPlayerState::STATE_GOTOFRAME);
}

void MediaPlayerStateMachine::receiveTargetFps(double fps) {
    m_PlayerParameters->m_fpsTarget = fps;
    static_cast<PStatePlay*>(m_States.value(IPlayerState::STATE_PLAY))->setFps(fps);
}

void MediaPlayerStateMachine::receivetoggleRecordImageStream() {
	if (m_stream)
		m_PlayerParameters->m_RecI = m_stream->toggleRecord();
	else
		m_PlayerParameters->m_RecI = false;
	emitSignals();
	return;
}

void MediaPlayerStateMachine::updatePlayerParameter() {

	stateParameters stateParam = m_CurrentPlayerState->getStateParameters();

	m_PlayerParameters->m_Back = stateParam.m_Back;
	m_PlayerParameters->m_Forw = stateParam.m_Forw;
	m_PlayerParameters->m_Paus = stateParam.m_Paus;
	m_PlayerParameters->m_Play = stateParam.m_Play;
	m_PlayerParameters->m_Stop = stateParam.m_Stop;

	m_PlayerParameters->m_CurrentFilename = m_CurrentPlayerState->getCurrentFileName();

	m_PlayerParameters->m_CurrentFrame = m_CurrentPlayerState->getCurrentFrame();
	m_PlayerParameters->m_CurrentFrameNumber = m_CurrentPlayerState->getCurrentFrameNumber();
	m_PlayerParameters->m_fpsSourceVideo = m_CurrentPlayerState->m_ImageStream->fps();
}

void MediaPlayerStateMachine::emitSignals() {

	Q_EMIT emitPlayerParameters(m_PlayerParameters);
}

void MediaPlayerStateMachine::setNextState(IPlayerState::PLAYER_STATES state) {
	m_NextPlayerState = m_States.value(state);

	Q_EMIT emitPlayerOperationDone();

}


