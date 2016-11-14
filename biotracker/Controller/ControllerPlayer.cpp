#include "ControllerPlayer.h"
#include "../biotracker_gui/biotracker/View/BioTracker3VideoControllWidget.h"
#include "../biotracker_gui/biotracker/View/BioTracker3MainWindow.h"
#include "Controller/ControllerTextureObject.h"

ControllerPlayer::ControllerPlayer(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{
    m_PlayerThread = new QThread(this);
}

void ControllerPlayer::loadImageStream(QString str) {
    // Save the path to the video file in Settings
    //m_Settings.setParam(CaptureParam::CAP_VIDEO_FILE, boost::filesystem::path(str.toStdString()));

    Q_EMIT emitLoadImageStream(str);
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
    IController *ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    ControllerTextureObject *ctrTexture = dynamic_cast<ControllerTextureObject *>(ctrA);

    ctrTexture->changeTextureModel(str);
}

void ControllerPlayer::connectViewToMainWindow(IController *controller)
{
    IView *view = controller->getView();
    static_cast<BioTracker3MainWindow *>(view)->addVideoControllWidget(m_View);

}

void ControllerPlayer::connectToOtherController(IController *controller)
{

}

void ControllerPlayer::callAnOtherController()
{

}

void ControllerPlayer::createModel()
{
    m_Model = new BioTracker3Player( this );
    m_Model->moveToThread(m_PlayerThread);
    m_PlayerThread->start();
}

void ControllerPlayer::createView()
{
    m_View = new BioTracker3VideoControllWidget(0, this, m_Model);
}

void ControllerPlayer::connectModelController()
{
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(m_Model);

    QObject::connect(player, &BioTracker3Player::emitPlayerOperationDone, this, &ControllerPlayer::handlePlayerResult);
//    QObject::connect(this, &ControllerPlayer::emitRunPlayerOperation, player, &BioTracker3Player::runPlayerOperation);


    QObject::connect(this, &ControllerPlayer::emitLoadImageStream, player, &BioTracker3Player::receiveLoadImageStreamCommand);
    QObject::connect(this, &ControllerPlayer::emitNextFrameCommand, player, &BioTracker3Player::receiveNextFramCommand);
    QObject::connect(this, &ControllerPlayer::emitPauseCommand, player, &BioTracker3Player::receivePauseCommand);
    QObject::connect(this, &ControllerPlayer::emitPlayCommand, player, &BioTracker3Player::receivePlayCommand);
    QObject::connect(this, &ControllerPlayer::emitPrevFrameCommand, player, &BioTracker3Player::receivePrevFrameCommand);
    QObject::connect(this, &ControllerPlayer::emitStopCommand, player, &BioTracker3Player::receiveStopCommand);
}

void ControllerPlayer::connectModelView()
{
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(m_Model);
    BioTracker3VideoControllWidget *widget = dynamic_cast<BioTracker3VideoControllWidget *>(m_View);

    QObject::connect(player, &BioTracker3Player::emitCurrentFrameNumber, widget, &BioTracker3VideoControllWidget::receiveCurrentFrameNumber);
    QObject::connect(player, &BioTracker3Player::emitFPS, widget, &BioTracker3VideoControllWidget::receiveFPS);
    QObject::connect(player, &BioTracker3Player::emitTotalNumbFrames, widget, &BioTracker3VideoControllWidget::receiveTotalNumbFrames);
    QObject::connect(player, &BioTracker3Player::emitVideoControllsStates, widget, &BioTracker3VideoControllWidget::receiveVideoControllsStates);

}

void ControllerPlayer::handlePlayerResult()
{
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(m_Model);
    player->runPlayerOperation();
}

void ControllerPlayer::handleNewCvMat(cv::Mat mat)
{
    ControllerTextureObject *ctr = dynamic_cast<ControllerTextureObject *> (m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT));
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(m_Model);
//    ctr->setCvMat(player->setCvMat(mat));
    // hier übergang zum texture object controller
}
