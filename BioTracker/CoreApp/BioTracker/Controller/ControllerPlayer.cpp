#include "ControllerPlayer.h"
#include "View/VideoControllWidget.h"
#include "View/MainWindow.h"
#include "Controller/ControllerTextureObject.h"
#include "Controller/ControllerPlugin.h"

ControllerPlayer::ControllerPlayer(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{
}

ControllerPlayer::~ControllerPlayer()
{
}

void ControllerPlayer::loadVideoStream(QString str) {

    qobject_cast<MediaPlayer*>(m_Model)->loadVideoStream(str);
}

void ControllerPlayer::loadPictures(std::vector<boost::filesystem::path> files) {
    qobject_cast<MediaPlayer*>(m_Model)->loadPictures(files);
}

void ControllerPlayer::loadCameraDevice(int i) {
    qobject_cast<MediaPlayer*>(m_Model)->loadCameraDevice(i);
}

void ControllerPlayer::nextFrame() {
    qobject_cast<MediaPlayer*>(m_Model)->nextFrameCommand();
}

void ControllerPlayer::prevFrame() {
    qobject_cast<MediaPlayer*>(m_Model)->prevFrameCommand();
}

void ControllerPlayer::play() {
    qobject_cast<MediaPlayer*>(m_Model)->playCommand();
}

void ControllerPlayer::stop() {
    qobject_cast<MediaPlayer*>(m_Model)->stopCommand();
}

void ControllerPlayer::pause() {
    qobject_cast<MediaPlayer*>(m_Model)->pauseCommand();
}

void ControllerPlayer::setGoToFrame(int frame) {
    qobject_cast<MediaPlayer*>(m_Model)->goToFrame(frame);
}

void ControllerPlayer::receiveRenderImage(std::shared_ptr<cv::Mat> mat, QString name) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    QPointer< ControllerTextureObject > ctrTextureObject = qobject_cast<ControllerTextureObject*>(ctr);

    ctrTextureObject->receiveCvMat(mat, name);
}

void ControllerPlayer::receiveImageToTracker(std::shared_ptr<cv::Mat> mat, uint number) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLUGIN);
    QPointer< ControllerPlugin > ctrPlugin = qobject_cast<ControllerPlugin*>(ctr);

    ctrPlugin->sendCurrentFrameToPlugin(mat, number);
}

void ControllerPlayer::changeImageView(QString str) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    QPointer< ControllerTextureObject > ctrTextureObject = qobject_cast<ControllerTextureObject*>(ctr);

    ctrTextureObject->changeTextureModel(str);
}

void ControllerPlayer::setTrackingActivated() {
    qobject_cast<MediaPlayer*>(m_Model)->setTrackingActive();
}

void ControllerPlayer::setTrackingDeactivated() {
    qobject_cast<MediaPlayer*>(m_Model)->setTrackingDeactive();
}

void ControllerPlayer::connectControllerToController() {
    IController* ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< MainWindow > mainWin = dynamic_cast<MainWindow*>(ctrM->getView());
    mainWin->addVideoControllWidget(m_View);
}

void ControllerPlayer::createModel() {
    m_Model = new MediaPlayer(this);
}

void ControllerPlayer::createView() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< MainWindow > mainWindow = dynamic_cast<MainWindow*>(ctr->getView());

    m_View = new VideoControllWidget(mainWindow, this, m_Model);
}

void ControllerPlayer::connectModelToController() {

    QObject::connect(qobject_cast<MediaPlayer*>(m_Model), &MediaPlayer::renderCurrentImage, this, &ControllerPlayer::receiveRenderImage);
    QObject::connect(qobject_cast<MediaPlayer*>(m_Model), &MediaPlayer::trackCurrentImage, this, &ControllerPlayer::receiveImageToTracker);

}
