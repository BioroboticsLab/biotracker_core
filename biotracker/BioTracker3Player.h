#ifndef BIOTRACKER3PLAYER_H
#define BIOTRACKER3PLAYER_H

#include <QObject>
#include "Interfaces/imodel.h"

#include "BioTracker3ImageStream.h"
#include <memory>
#include "QString"
#include "QMap"

#include "../biotracker_gui/biotracker/BioTracker3VideoControllWidget.h"
#include "../biotracker_gui/biotracker/BioTracker3VideoView.h"

#include "Interfaces/IPlayerState.h"


class BioTracker3Player : public IModel {
    Q_OBJECT
  public:
    explicit BioTracker3Player(BioTracker::Core::BioTracker3TextureObject *textureObject);



    // Interface for the States
    bool getStateOfForward();
    bool getStateOfPlay();
    bool getStateOfRew();
    bool getStateOfStop();

    IModel *getStateModel();

  public Q_SLOTS:

    void loadVideo(QString str);
    void nextFrame();
    void play();

    void setNextState(IPlayerState::PLAYER_STATES state);
    void setCurrentState(IPlayerState::PLAYER_STATES state);

  Q_SIGNALS:
    void changeImageStream(std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

  private:
    void setupVideoControllWidget();

  private:
    IPlayerState *m_CurrentPlayerState;

    QMap<IPlayerState::PLAYER_STATES, IPlayerState *> m_States;

    IModel *m_TextureObjectModel;
    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> m_ImageStream;
};

#endif // BIOTRACKER3PLAYER_H
