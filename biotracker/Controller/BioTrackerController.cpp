#include "BioTrackerController.h"

#include "Model/BioTrackerTrackingAlgorithm.h"
#include "Model/BioTracker3TextureObject.h"

#include "../biotracker_gui/biotracker/BioTracker3VideoControllWidget.h"
#include "../biotracker_gui/biotracker/BioTracker3VideoView.h"


BioTrackerController::BioTrackerController() :
    m_TextureObjectModel(new BioTracker::Core::BioTracker3TextureObject())

{
    createApplication();
}

void BioTrackerController::createApplication() {
    createBioTrackerMainWindow();
    createBioTrackerPlayer();
    createTrackingAlgorithm();
}

void BioTrackerController::loadVideo(QString str) {
    // Save the path to the video file in Settings
    m_Settings.setParam(CaptureParam::CAP_VIDEO_FILE, boost::filesystem::path(str.toStdString()));

    Q_EMIT s_LoadVideo(str);
    Q_EMIT s_Operate();
}

void BioTrackerController::nextFrame() {
    Q_EMIT s_NextFrame();
    Q_EMIT s_Operate();
}

void BioTrackerController::prevFrame() {
    Q_EMIT s_PrevFrame();
    Q_EMIT s_Operate();
}

void BioTrackerController::play() {
    Q_EMIT s_Play();
    Q_EMIT s_Operate();
}

void BioTrackerController::stop() {
    Q_EMIT s_Stop();
    Q_EMIT s_Operate();
}

void BioTrackerController::pause() {
    Q_EMIT s_Pause();
    Q_EMIT s_Operate();
}

void BioTrackerController::createBioTrackerPlayer() {
    m_Player = new BioTracker3Player(m_TextureObjectModel);

    QObject::connect(this, &BioTrackerController::s_LoadVideo, m_Player, &BioTracker3Player::loadVideo,
                     Qt::BlockingQueuedConnection);
    QObject::connect(this, &BioTrackerController::s_NextFrame, m_Player, &BioTracker3Player::nextFrame,
                     Qt::BlockingQueuedConnection);
    QObject::connect(this, &BioTrackerController::s_PrevFrame, m_Player, &BioTracker3Player::prevFrame,
                     Qt::BlockingQueuedConnection);
    QObject::connect(this, &BioTrackerController::s_Play, m_Player, &BioTracker3Player::play, Qt::BlockingQueuedConnection);
    QObject::connect(this, &BioTrackerController::s_Stop, m_Player, &BioTracker3Player::stop, Qt::BlockingQueuedConnection);
    QObject::connect(this, &BioTrackerController::s_Pause, m_Player, &BioTracker3Player::pause,
                     Qt::BlockingQueuedConnection);
    QObject::connect(this, &BioTrackerController::s_Operate, m_Player, &BioTracker3Player::operate);
    QObject::connect(m_Player, &BioTracker3Player::emitStateChangeDone, this, &BioTrackerController::handlePlayerOperation);

    m_Player->moveToThread(&m_PlayerThread);
    m_PlayerThread.start();

    m_videoControlWidget = new BioTracker3VideoControllWidget(this, dynamic_cast<IModel *>(m_Player));
    addView(m_Enum.valueToKey(VIEWS::VIDEOCONTROL), m_videoControlWidget);

    m_VideoView = new BioTracker3VideoView(0, this, dynamic_cast<IModel *>(m_TextureObjectModel));
    addView(m_Enum.valueToKey(VIEWS::VIDEOVIEW), m_VideoView);

    static_cast<BioTracker3MainWindow *>(m_MainWindow)->addVideoControllWidget(m_videoControlWidget);
    static_cast<BioTracker3MainWindow *>(m_MainWindow)->addVideoView(m_VideoView);
}

void BioTrackerController::createBioTrackerMainWindow() {
    m_MainWindow = new BioTracker3MainWindow(0, this);
    addView(m_Enum.valueToKey(VIEWS::MAINWINDOW), m_MainWindow);
    static_cast<BioTracker3MainWindow *>(m_MainWindow)->show();
}

void BioTrackerController::createTrackingAlgorithm() {

}

void BioTrackerController::handlePlayerOperation() {
    Q_EMIT s_Operate();
}

void BioTrackerController::handleTrackerResult(BioTracker3TrackedObject trackedObject) {

}
