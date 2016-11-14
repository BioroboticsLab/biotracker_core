#ifndef BIOTRACKER3PLAYER_H
#define BIOTRACKER3PLAYER_H

#include <QObject>
#include "Interfaces/imodel.h"

#include "BioTracker3ImageStream.h"
#include <memory>
#include "QString"
#include "QMap"
#include "QThread"
#include "opencv2/core/core.hpp"

#include "../biotracker_gui/biotracker/View/BioTracker3VideoControllWidget.h"
#include "../biotracker_gui/biotracker/View/BioTracker3VideoView.h"

#include "Interfaces/IPlayerState.h"


class BioTracker3Player : public IModel {
    Q_OBJECT
  public:
    explicit BioTracker3Player(QObject *parent = 0);

    void runPlayerOperation();


    void setNextState(IPlayerState::PLAYER_STATES state);

  public Q_SLOTS:
    void receiveLoadImageStreamCommand(QString fileDir);
    void receivePrevFrameCommand();
    void receiveNextFramCommand();
    void receivePauseCommand();
    void receiveStopCommand();
    void receivePlayCommand();

    void receiveStateDone();


  Q_SIGNALS:
    void emitMediaType(GuiParam::MediaType type);
    void emitTotalNumbFrames(size_t num);
    void emitCurrentFrameNumber(size_t num);
    void emitFPS(double fps);
    void emitCurrentFileName(QString name);
    void emitCurrentFrame(cv::Mat mat, QString name);
    void emitVideoControllsStates(QVector<bool> states);

    void emitPlayerOperationDone();

  private:
    void updatePlayerParameter();
    void emitSignals();


  private:
    IPlayerState *m_CurrentPlayerState;
    QThread m_StateThread;
    QMap<IPlayerState::PLAYER_STATES, IPlayerState *> m_States;
    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> m_ImageStream;

    GuiParam::MediaType m_MediaType;
    size_t m_TotalNumbFrames;
    size_t m_CurrentFrameNumber;
    double m_fps;
    QString m_CurrentFilename;
    cv::Mat m_CurrentFrame;
    QVector<bool> m_VideoControllsStates;

    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
    bool m_Paus;

    QString m_NameOfCvMat = "Original";
};


#endif // BIOTRACKER3PLAYER_H
