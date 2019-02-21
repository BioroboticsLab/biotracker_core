#include "ControllerPlayer.h"
#include "View/VideoControllWidget.h"
#include "View/MainWindow.h"
#include "Controller/ControllerTextureObject.h"
#include "Controller/ControllerPlugin.h"
#include "Controller/ControllerGraphicScene.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "Controller/ControllerCoreParameter.h"
#include "Controller/ControllerMainWindow.h"


#include <QGraphicsItem>
#include <QToolButton>

ControllerPlayer::ControllerPlayer(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IControllerCfg(parent, context, ctr)
{
}

ControllerPlayer::~ControllerPlayer()
{
}

void ControllerPlayer::loadVideoStream(std::vector<boost::filesystem::path> files) {
    qobject_cast<MediaPlayer*>(m_Model)->loadVideoStream(files);
	emitPauseState(true);
}

void ControllerPlayer::loadPictures(std::vector<boost::filesystem::path> files) {
    qobject_cast<MediaPlayer*>(m_Model)->loadPictures(files);
	emitPauseState(true);
}

void ControllerPlayer::loadCameraDevice(CameraConfiguration conf) {
    qobject_cast<MediaPlayer*>(m_Model)->loadCameraDevice(conf);
	emitPauseState(true);
}

void ControllerPlayer::nextFrame() {
    qobject_cast<MediaPlayer*>(m_Model)->nextFrameCommand();
}

void ControllerPlayer::prevFrame() {
    qobject_cast<MediaPlayer*>(m_Model)->prevFrameCommand();
}

void ControllerPlayer::play() {
    qobject_cast<MediaPlayer*>(m_Model)->playCommand();
	emitPauseState(false);
}

void ControllerPlayer::stop() {
    qobject_cast<MediaPlayer*>(m_Model)->stopCommand();
	emitPauseState(true);
}

void ControllerPlayer::pause() {
    qobject_cast<MediaPlayer*>(m_Model)->pauseCommand();
	emitPauseState(true);
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

int ControllerPlayer::recordOutput() {
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	QPointer< ControllerGraphicScene > ctrTextureObject = qobject_cast<ControllerGraphicScene*>(ctr);

	return qobject_cast<MediaPlayer*>(m_Model)->toggleRecordGraphicsScenes(dynamic_cast <GraphicsView *> (ctrTextureObject->getView()));
}

int ControllerPlayer::recordInput() {
	return qobject_cast<MediaPlayer*>(m_Model)->toggleRecordImageStream();
}

void ControllerPlayer::setTargetFps(double fps) {
    return qobject_cast<MediaPlayer*>(m_Model)->setTargetFPS(fps);
}

QString ControllerPlayer::takeScreenshot() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
    QPointer< ControllerGraphicScene > ctrTextureObject = qobject_cast<ControllerGraphicScene*>(ctr);
    return qobject_cast<MediaPlayer*>(m_Model)->takeScreenshot(dynamic_cast<GraphicsView *>(ctrTextureObject->getView()));
}

void ControllerPlayer::setTrackingActivated() {
    qobject_cast<MediaPlayer*>(m_Model)->setTrackingActive();
}

void ControllerPlayer::setTrackingDeactivated() {
    qobject_cast<MediaPlayer*>(m_Model)->setTrackingDeactive();
}

void ControllerPlayer::connectControllerToController() {
	//connect to mainwindow
    IController* ictrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    ControllerMainWindow* ctrM = qobject_cast<ControllerMainWindow*>(ictrM);
    QPointer< MainWindow > mainWin = dynamic_cast<MainWindow*>(ictrM->getView());
    mainWin->addVideoControllWidget(m_View);
    VideoControllWidget* vControl = static_cast<VideoControllWidget*>(m_View);
    vControl->setupVideoToolbar();

    //QObject::connect(qobject_cast<MediaPlayer*>(m_Model), &MediaPlayer::emitNextMediaInBatchLoaded, vControl, &VideoControllWidget::videoChanged);
    QObject::connect(ctrM, &ControllerMainWindow::emitMediaLoaded, vControl, &VideoControllWidget::videoChanged);
    QObject::connect(ctrM, &ControllerMainWindow::emitFilesCount, vControl, &VideoControllWidget::getMaxBatchNumber);


	////connect to coreparameterview
	//IController* ictrCpv = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
	//QPointer< ControllerCoreParameter > ctrCpv = dynamic_cast<ControllerCoreParameter*>(ictrCpv);

	//QObject::connect(this, &ControllerPlayer::emitPauseState, ctrCpv, &ControllercoreParameter)
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
    QObject::connect(this, &ControllerPlayer::emitPauseState, qobject_cast<MediaPlayer*>(m_Model), &MediaPlayer::rcvPauseState);
	QObject::connect(qobject_cast<MediaPlayer*>(m_Model), &MediaPlayer::signalVisualizeCurrentModel, this, &ControllerPlayer::receiveVisualizeCurrentModel);
    QObject::connect(qobject_cast<MediaPlayer*>(m_Model), &MediaPlayer::signalCurrentFrameNumberToPlugin, this, &ControllerPlayer::receiveCurrentFrameNumberToPlugin);

    QObject::connect(qobject_cast<MediaPlayer*>(m_Model), &MediaPlayer::emitNextMediaInBatch, this, &ControllerPlayer::receiveNextMediaInBatch, Qt::DirectConnection);
    QObject::connect(qobject_cast<MediaPlayer*>(m_Model), &MediaPlayer::emitNextMediaInBatchLoaded, this, &ControllerPlayer::receiveNextMediaInBatchLoaded, Qt::DirectConnection);

    
    IController* ctrTRCC = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
    QPointer< ControllerTrackedComponentCore > trCC = qobject_cast<ControllerTrackedComponentCore*>(ctrTRCC);

    QObject::connect(trCC, &ControllerTrackedComponentCore::emitTrackNumber, this, &ControllerPlayer::receiveTrackCount, Qt::DirectConnection);

}

void ControllerPlayer::receiveNextMediaInBatch(const std::string path){
    _trackCountEndOfBatch = _trackCount;
    Q_EMIT emitNextMediaInBatch(path);
}

void ControllerPlayer::receiveNextMediaInBatchLoaded(const std::string path){
    Q_EMIT emitNextMediaInBatchLoaded(path);
    
    IController* ctrTRCC = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
    QPointer< ControllerTrackedComponentCore > trCC = qobject_cast<ControllerTrackedComponentCore*>(ctrTRCC);
    for(int i=0; i<_trackCountEndOfBatch; i++){
        trCC->emitAddTrack();
    }

    //set video name in video control widget
    VideoControllWidget* vControl = static_cast<VideoControllWidget*>(m_View);
    vControl->videoChanged(path);
}

void ControllerPlayer::receiveTrackCount(int trackNo){
    _trackCount = trackNo;
}


void ControllerPlayer::receiveVisualizeCurrentModel(uint frameNumber)
{
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
	QPointer< ControllerTrackedComponentCore > ctrTrCompCore = qobject_cast<ControllerTrackedComponentCore*>(ctr);

	ctrTrCompCore->receiveVisualizeTrackingModel(frameNumber);
}

void ControllerPlayer::receiveChangeDisplayImage(QString str) {

	VideoControllWidget *w = dynamic_cast<VideoControllWidget*>(m_View);
	w->setSelectedView(str);
}

void ControllerPlayer::receiveCurrentFrameNumberToPlugin(uint frameNumber)
{
	Q_EMIT signalCurrentFrameNumberToPlugin(frameNumber);
}
