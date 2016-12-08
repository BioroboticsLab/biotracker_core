#ifndef CONTROLLERPLAYER_H
#define CONTROLLERPLAYER_H

#include "QThread"
#include "Interfaces/IController/IController.h"
#include "Model/MediaPlayer.h"
#include "QPointer"


class ControllerPlayer : public IController
{
    Q_OBJECT
public:
    ControllerPlayer(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
    ~ControllerPlayer();

    void loadVideoStream(QString str);
    void loadPictures(std::vector<boost::filesystem::path> files);
    void loadCameraDevice(int i);

    void nextFrame();
    void prevFrame();
    void play();
    void stop();
    void pause();


    void changeImageView(QString str);
    void setTrackingActivated();
    void setTrackingDeactivated();

    // IController interface
public:
    void connectControllerToController() override;

protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;

Q_SIGNALS:
  void emitLoadVideoStream(QString str);
  void emitLoadPictures(std::vector<boost::filesystem::path> files);
  void emitLoadCameraDevice(int i);
  void emitActivateTracking();
  void emitDeactivateTracking();


  void emitNextFrameCommand();
  void emitPrevFrameCommand();
  void emitPlayCommand();
  void emitStopCommand();
  void emitPauseCommand();

  void emitRunPlayerOperation();


public Q_SLOTS:
  void receivePlayerOperationDone();
  void receiveTrackingOperationDone();

  void receiveCurrentFrameNumber(size_t num);
  void receiveCurrentFrameStr(std::shared_ptr<cv::Mat> mat, QString name);
  void receiveFPS(double fps);
  void receiveTotalNumbFrames(size_t num);
  void receiveVideoControllsStates(QVector<bool> states);
  void receiveTrackingIsActiveState(bool state);

private:
    QPointer< QThread > m_PlayerThread;

    QPointer< IModel > m_Player;

    bool m_TrackingIsActive;
};

#endif // CONTROLLERPLAYER_H
