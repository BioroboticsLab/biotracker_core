#include "ControllerPlayer.h"
#include "View/VideoControllWidget.h"
#include "View/MainWindow.h"
#include "Controller/ControllerTextureObject.h"

ControllerPlayer::ControllerPlayer(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{
	m_TrackingIsActive = false; //TODO is this correct?
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
    IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    QPointer< ControllerTextureObject > ctrTextureObject = qobject_cast<ControllerTextureObject *>(ctr);

    ctrTextureObject->changeTextureModel(str);
}

void ControllerPlayer::setTrackingActivated()
{
    m_TrackingIsActive = true;
}

void ControllerPlayer::setTrackingDeactivated()
{
    m_TrackingIsActive = false;
}

void ControllerPlayer::connectControllerToController()
{
    IController * ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< MainWindow > mainWin = dynamic_cast<MainWindow *>(ctrM->getView());
    mainWin->addVideoControllWidget(m_View);
}

void ControllerPlayer::createModel()
{
    // Do net set a Parent in order to run the Player in the QThread!
    m_Model = new MediaPlayer(  );
    m_Model->moveToThread(m_PlayerThread);
    m_PlayerThread->start();
}

void ControllerPlayer::createView()
{
    IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< MainWindow > mainWindow = dynamic_cast<MainWindow *>(ctr->getView());

    m_View = new VideoControllWidget(mainWindow, this, m_Model);
}

void ControllerPlayer::connectModelToController()
{
    QPointer< MediaPlayer > player = qobject_cast<MediaPlayer *>(m_Model);

    //Load ImageStream
    QObject::connect(this, &ControllerPlayer::emitLoadVideoStream, player, &MediaPlayer::receiveLoadVideoCommand);
    QObject::connect(this, &ControllerPlayer::emitLoadCameraDevice, player, &MediaPlayer::receiveLoadCameraDevice);
    QObject::connect(this, &ControllerPlayer::emitLoadPictures, player, &MediaPlayer::receiveLoadPictures);

    // Set TrackingActive
    QObject::connect(this, &ControllerPlayer::emitActivateTracking, player, &MediaPlayer::receiveActivateTracking);
    QObject::connect(this, &ControllerPlayer::emitDeactivateTracking, player, &MediaPlayer::receiveDeaktivateTracking);


    // Controll the Player
    QObject::connect(this, &ControllerPlayer::emitNextFrameCommand, player, &MediaPlayer::receiveNextFramCommand);
    QObject::connect(this, &ControllerPlayer::emitPauseCommand, player, &MediaPlayer::receivePauseCommand);
    QObject::connect(this, &ControllerPlayer::emitPlayCommand, player, &MediaPlayer::receivePlayCommand);
    QObject::connect(this, &ControllerPlayer::emitPrevFrameCommand, player, &MediaPlayer::receivePrevFrameCommand);
    QObject::connect(this, &ControllerPlayer::emitStopCommand, player, &MediaPlayer::receiveStopCommand);


    // Handel Player results
    QObject::connect(player, &MediaPlayer::emitCurrentFrameNumber, this, &ControllerPlayer::receiveCurrentFrameNumber, Qt::BlockingQueuedConnection);
    QObject::connect(player, &MediaPlayer::emitFPS, this, &ControllerPlayer::receiveFPS, Qt::BlockingQueuedConnection);
    QObject::connect(player, &MediaPlayer::emitTotalNumbFrames, this, &ControllerPlayer::receiveTotalNumbFrames, Qt::BlockingQueuedConnection);
    QObject::connect(player, &MediaPlayer::emitVideoControllsStates, this, &ControllerPlayer::receiveVideoControllsStates, Qt::BlockingQueuedConnection);
    QObject::connect(player, &MediaPlayer::emitCurrentFrameStr, this, &ControllerPlayer::receiveCurrentFrameStr, Qt::BlockingQueuedConnection);

    QObject::connect(player, &MediaPlayer::emitTrackingIsActiveState, this, &ControllerPlayer::receiveTrackingIsActiveState, Qt::BlockingQueuedConnection);

    QObject::connect(player, &MediaPlayer::emitPlayerOperationDone, this, &ControllerPlayer::receivePlayerOperationDone);

    QObject::connect(this, &ControllerPlayer::emitRunPlayerOperation, player, &MediaPlayer::runPlayerOperation);
}

void ControllerPlayer::receivePlayerOperationDone()
{
	//TODO Andi This is a major issue: Without adding the next line the video will not go on windows.
	//To me it seems some variables are initialized wrong (due to compiler specs / default init instead of zero init in >VS2013)
	//m_TrackingIsActive = false;
    if(! m_TrackingIsActive)
        Q_EMIT emitRunPlayerOperation();
}

void ControllerPlayer::receiveTrackingOperationDone()
{
    if(m_TrackingIsActive)
        Q_EMIT emitRunPlayerOperation();

}

void ControllerPlayer::receiveCurrentFrameNumber(size_t num)
{
    QPointer< VideoControllWidget > widget = static_cast<VideoControllWidget *>(m_View);

    widget->setCurrentFrameNumber(num);
}

void ControllerPlayer::receiveCurrentFrameStr(std::shared_ptr<cv::Mat> mat, QString name)
{
    IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    qobject_cast<ControllerTextureObject *>(ctr)->receiveCvMat(mat, name);
}

void ControllerPlayer::receiveFPS(double fps)
{
    QPointer< VideoControllWidget > widget = static_cast<VideoControllWidget *>(m_View);

    widget->setFPS(fps);
}

void ControllerPlayer::receiveTotalNumbFrames(size_t num)
{
    QPointer< VideoControllWidget > widget = static_cast<VideoControllWidget *>(m_View);

    widget->setTotalNumbFrames(num);
}

void ControllerPlayer::receiveVideoControllsStates(QVector<bool> states)
{
    QPointer< VideoControllWidget > widget = static_cast<VideoControllWidget *>(m_View);

    widget->setVideoControllsStates(states);
}

void ControllerPlayer::receiveTrackingIsActiveState(bool state)
{
    m_TrackingIsActive = state;
}
