#include "MediaPlayer.h"

MediaPlayer::MediaPlayer(QObject* parent) :
    IModel(parent) {

    m_TrackingIsActive = false;
    // Initialize PlayerStateMachine and a Thread for the Player
    //    // Do not set a Parent for MediaPlayerStateMachine in order to run the Player in the QThread!

    m_PlayerThread = new QThread(this);
    m_Player = new MediaPlayerStateMachine();

    // Connect MediaPlayer with PlayerStateMachine

    // Load ImageStreams in StateMachine
    QObject::connect(this, &MediaPlayer::loadVideoStream, m_Player, &MediaPlayerStateMachine::receiveLoadVideoCommand);
    QObject::connect(this, &MediaPlayer::loadCameraDevice, m_Player, &MediaPlayerStateMachine::receiveLoadCameraDevice);
    QObject::connect(this, &MediaPlayer::loadPictures, m_Player, &MediaPlayerStateMachine::receiveLoadPictures);

    // Controll the Player
    QObject::connect(this, &MediaPlayer::nextFrameCommand, m_Player, &MediaPlayerStateMachine::receiveNextFramCommand);
    QObject::connect(this, &MediaPlayer::pauseCommand, m_Player, &MediaPlayerStateMachine::receivePauseCommand);
    QObject::connect(this, &MediaPlayer::playCommand, m_Player, &MediaPlayerStateMachine::receivePlayCommand);
    QObject::connect(this, &MediaPlayer::prevFrameCommand, m_Player, &MediaPlayerStateMachine::receivePrevFrameCommand);
    QObject::connect(this, &MediaPlayer::stopCommand, m_Player, &MediaPlayerStateMachine::receiveStopCommand);
    QObject::connect(this, &MediaPlayer::goToFrame, m_Player, &MediaPlayerStateMachine::receiveGoToFrame);

    // Handel PlayerStateMachine results
    QObject::connect(m_Player, &MediaPlayerStateMachine::emitPlayerParameters, this, &MediaPlayer::receivePlayerParameters, Qt::BlockingQueuedConnection);

    // Handle next state operation
    QObject::connect(m_Player, &MediaPlayerStateMachine::emitPlayerOperationDone, this, &MediaPlayer::receivePlayerOperationDone);
    QObject::connect(this, &MediaPlayer::runPlayerOperation, m_Player, &MediaPlayerStateMachine::receiveRunPlayerOperation);

    // Move the PlayerStateMachine to the Thread
    m_Player->moveToThread(m_PlayerThread);

    // Start the Thread
    m_PlayerThread->start();
}

MediaPlayer::~MediaPlayer() {
    m_PlayerThread->quit();
    m_PlayerThread->wait();
}

void MediaPlayer::setTrackingActive() {
    m_TrackingIsActive = true;
}

void MediaPlayer::setTrackingDeactive() {
    m_TrackingIsActive = false;
}

bool MediaPlayer::getPlayState() {
    return m_Play;
}

bool MediaPlayer::getForwardState() {
    return m_Forw;
}

bool MediaPlayer::getBackwardState() {
    return m_Back;
}

bool MediaPlayer::getStopState() {
    return m_Stop;
}

bool MediaPlayer::getPauseState() {
    return m_Paus;
}

bool MediaPlayer::getTrackingState() {
    return m_TrackingIsActive;
}

size_t MediaPlayer::getTotalNumberOfFrames() {
    return m_TotalNumbFrames;
}

size_t MediaPlayer::getCurrentFrameNumber() {
    return m_CurrentFrameNumber;
}

double MediaPlayer::getFpsOfSourceFile() {
    return m_fpsOfSourceFile;
}

double MediaPlayer::getCurrentFPS() {
    return m_currentFPS;
}

QString MediaPlayer::getCurrentFileName() {
    return m_CurrentFilename;
}

std::shared_ptr<cv::Mat> MediaPlayer::getCurrentFrame() {
    return m_CurrentFrame;
}

void MediaPlayer::receivePlayerParameters(playerParameters* param) {

    m_Back = param->m_Back;
    m_Paus = param->m_Paus;
    m_Play = param->m_Play;
    m_Stop = param->m_Stop;
    m_Forw = param->m_Forw;

    m_CurrentFilename = param->m_CurrentFilename;
    m_CurrentFrame = param->m_CurrentFrame;
    m_CurrentFrameNumber = param->m_CurrentFrameNumber;
    m_fpsOfSourceFile = param->m_fpsSourceVideo;
    m_TotalNumbFrames = param->m_TotalNumbFrames;

    m_CurrentFrame = param->m_CurrentFrame;

    Q_EMIT renderCurrentImage(m_CurrentFrame, m_NameOfCvMat);

    if(m_TrackingIsActive)
        Q_EMIT trackCurrentImage(m_CurrentFrame, m_CurrentFrameNumber);
    Q_EMIT notifyView();
}


void MediaPlayer::receivePlayerOperationDone() {
    // Only emit this SIGNL when tracking is not active
    //if(! m_TrackingIsActive)



    end = std::chrono::steady_clock::now();
    std::cout << "Printing took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "us.\n";


    Q_EMIT runPlayerOperation();

    start = std::chrono::steady_clock::now();
}

void MediaPlayer::receiveTrackingOperationDone() {
    // Only emit this SIGNAL when tracking is active
    if(m_TrackingIsActive)
        Q_EMIT runPlayerOperation();

}
