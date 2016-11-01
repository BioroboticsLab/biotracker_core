#ifndef BIOTRACKERCONTROLLER_H
#define BIOTRACKERCONTROLLER_H
#include "Interfaces/icontroller.h"
#include "settings/Settings.h"
#include "ImageStream.h"
#include <memory>
#include "QThread"

#include "Model/BioTracker3Player.h"
#include "Model/BioTrackerTrackingAlgorithm.h"


class BioTrackerController : public IController {
    Q_OBJECT

  public:
    enum VIEWS {
        MAINWINDOW, VIDEOVIEW, VIDEOCONTROL
    };
    enum MODELS {
        PLAYER, OBJECTMODEL
    };
    Q_ENUM(VIEWS)
    Q_ENUM(MODELS)
    BioTrackerController();

    void loadVideo(QString str);
    void nextFrame();
    void prevFrame();
    void play();
    void stop();
    void pause();

    IModel *getPlayer();

  private:
    void createApplication();
    void createBioTrackerPlayer();
    void createBioTrackerMainWindow();
    void createTrackingAlgorithm();

  Q_SIGNALS:
    void s_LoadVideo(QString str);
    void s_NextFrame();
    void s_PrevFrame();
    void s_Play();
    void s_Stop();
    void s_Operate();
    void s_Pause();

  public Q_SLOTS:
    void handlePlayerOperation();


  private:
    BioTracker::Core::Settings m_Settings;
    BioTracker::Core::BioTracker3TextureObject *m_TextureObjectModel;

    IView *m_VideoView;
    IView *m_videoControlWidget;
    IView *m_MainWindow;

    BioTracker3Player *m_Player;

    QThread m_PlayerThread;
    QMetaEnum m_Enum = QMetaEnum::fromType<VIEWS>();

    BioTrackerTrackingAlgorithm m_TrackingAlgorithm;
    QThread m_TrackingThread;

    IController *m_TrackingController;
};

#endif // BIOTRACKERCONTROLLER_H
