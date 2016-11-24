#include "ControllerPlayer.h"
#include "View/BioTracker3VideoControllWidget.h"
#include "View/BioTracker3MainWindow.h"
#include "Controller/ControllerTextureObject.h"

ControllerPlayer::ControllerPlayer(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{
    m_PlayerThread = new QThread(this);
}

ControllerPlayer::~ControllerPlayer()
{
    m_PlayerThread->quit();
    m_PlayerThread->wait();
}

void ControllerPlayer::loadVideoStream(QString str) {
    // Save the path to the video file in Settings
    //m_Settings.setParam(CaptureParam::CAP_VIDEO_FILE, boost::filesystem::path(str.toStdString()));

    Q_EMIT emitLoadVideoStream(str);
}

void ControllerPlayer::loadPictures(std::vector<boost::filesystem::path> files)
{
    Q_EMIT emitLoadPictures(files);
}

void ControllerPlayer::loadCameraDevice(int i)
{
    Q_EMIT emitLoadCameraDevice(i);
}

void ControllerPlayer::nextFrame() {
    Q_EMIT emitNextFrameCommand();
}

void ControllerPlayer::prevFrame() {
    Q_EMIT emitPrevFrameCommand();
}

void ControllerPlayer::play() {
    Q_EMIT emitPlayCommand();
}

void ControllerPlayer::stop() {
    Q_EMIT emitStopCommand();
}

void ControllerPlayer::pause() {
    Q_EMIT emitPauseCommand();
}

void ControllerPlayer::changeImageView(QString str)
{
    QPointer< IController > ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    QPointer< ControllerTextureObject > ctrTextureObject = qobject_cast<ControllerTextureObject *>(ctr);

    ctrTextureObject->changeTextureModel(str);
}

void ControllerPlayer::connectController()
{
    QPointer< IController > ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< BioTracker3MainWindow > mainWin = dynamic_cast<BioTracker3MainWindow *>(ctrM->getView());
    mainWin->addVideoControllWidget(m_View);
}

void ControllerPlayer::createModel()
{
    // Do net set a Parent in order to run the Player in the QThread!
    m_Model = new BioTracker3Player(  );
    m_Model->moveToThread(m_PlayerThread);
    m_PlayerThread->start();
}

void ControllerPlayer::createView()
{
    QPointer< IController > ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< BioTracker3MainWindow > mainWindow = dynamic_cast<BioTracker3MainWindow *>(ctr->getView());

    m_View = new BioTracker3VideoControllWidget(mainWindow, this, m_Model);
}

void ControllerPlayer::connectModelController()
{
    QPointer< BioTracker3Player > player = qobject_cast<BioTracker3Player *>(m_Model);

    //Load ImageStream
    QObject::connect(this, &ControllerPlayer::emitLoadVideoStream, player, &BioTracker3Player::receiveLoadVideoCommand);
    QObject::connect(this, &ControllerPlayer::emitLoadCameraDevice, player, &BioTracker3Player::receiveLoadCameraDevice);
    QObject::connect(this, &ControllerPlayer::emitLoadPictures, player, &BioTracker3Player::receiveLoadPictures);


    // Controll the Player
    QObject::connect(this, &ControllerPlayer::emitNextFrameCommand, player, &BioTracker3Player::receiveNextFramCommand);
    QObject::connect(this, &ControllerPlayer::emitPauseCommand, player, &BioTracker3Player::receivePauseCommand);
    QObject::connect(this, &ControllerPlayer::emitPlayCommand, player, &BioTracker3Player::receivePlayCommand);
    QObject::connect(this, &ControllerPlayer::emitPrevFrameCommand, player, &BioTracker3Player::receivePrevFrameCommand);
    QObject::connect(this, &ControllerPlayer::emitStopCommand, player, &BioTracker3Player::receiveStopCommand);


    // Handel Player results
    QObject::connect(player, &BioTracker3Player::emitCurrentFrameNumber, this, &ControllerPlayer::receiveCurrentFrameNumber, Qt::BlockingQueuedConnection);
    QObject::connect(player, &BioTracker3Player::emitFPS, this, &ControllerPlayer::receiveFPS, Qt::BlockingQueuedConnection);
    QObject::connect(player, &BioTracker3Player::emitTotalNumbFrames, this, &ControllerPlayer::receiveTotalNumbFrames, Qt::BlockingQueuedConnection);
    QObject::connect(player, &BioTracker3Player::emitVideoControllsStates, this, &ControllerPlayer::receiveVideoControllsStates, Qt::BlockingQueuedConnection);

    QObject::connect(player, &BioTracker3Player::emitPlayerOperationDone, this, &ControllerPlayer::receivePlayerOperationDone);

    QObject::connect(this, &ControllerPlayer::emitRunPlayerOperation, player, &BioTracker3Player::runPlayerOperation);
}

void ControllerPlayer::receivePlayerOperationDone()
{
    Q_EMIT emitRunPlayerOperation();
}

void ControllerPlayer::receiveCurrentFrameNumber(size_t num)
{
    QPointer< BioTracker3VideoControllWidget > widget = static_cast<BioTracker3VideoControllWidget *>(m_View);

    widget->setCurrentFrameNumber(num);
}

void ControllerPlayer::receiveFPS(double fps)
{
    QPointer< BioTracker3VideoControllWidget > widget = static_cast<BioTracker3VideoControllWidget *>(m_View);

    widget->setFPS(fps);
}

void ControllerPlayer::receiveTotalNumbFrames(size_t num)
{
    QPointer< BioTracker3VideoControllWidget > widget = static_cast<BioTracker3VideoControllWidget *>(m_View);

    widget->setTotalNumbFrames(num);
}

void ControllerPlayer::receiveVideoControllsStates(QVector<bool> states)
{
    QPointer< BioTracker3VideoControllWidget > widget = static_cast<BioTracker3VideoControllWidget *>(m_View);

    widget->setVideoControllsStates(states);
}
