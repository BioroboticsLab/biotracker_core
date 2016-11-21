#ifndef CONTROLLERPLAYER_H
#define CONTROLLERPLAYER_H

#include "QThread"
#include "Interfaces/IController/icontroller.h"
#include "Model/BioTracker3Player.h"
#include "QPointer"


class ControllerPlayer : public IController
{
    Q_OBJECT
public:
    ControllerPlayer(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void loadVideoStream(QString str);
    void loadPictures(std::vector<boost::filesystem::path> files);

    void nextFrame();
    void prevFrame();
    void play();
    void stop();
    void pause();


    void changeImageView(QString str);

    // IController interface
public:
    void connectController() override;

protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;

Q_SIGNALS:
  void emitLoadVideoStream(QString str);
  void emitLoadPictures(std::vector<boost::filesystem::path> files);
  void emitNextFrameCommand();
  void emitPrevFrameCommand();
  void emitPlayCommand();
  void emitStopCommand();
  void emitPauseCommand();

  void emitRunPlayerOperation();


public Q_SLOTS:
  void handlePlayerResult();

  void receiveCurrentFrameNumber(size_t num);
  void receiveFPS(double fps);
  void receiveTotalNumbFrames(size_t num);
  void receiveVideoControllsStates(QVector<bool> states);

private:
    QPointer< QThread > m_PlayerThread;

    QPointer< IModel > m_Player;
};

#endif // CONTROLLERPLAYER_H
