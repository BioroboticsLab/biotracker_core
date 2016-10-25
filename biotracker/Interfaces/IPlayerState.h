#ifndef IPLAYERSTATE_H
#define IPLAYERSTATE_H

#include <QObject>

#include "Interfaces/imodel.h"

#include "Model/BioTracker3ImageStream.h"
#include "Model/BioTracker3TextureObject.h"

class BioTracker3Player;
class IPlayerState : public IModel {
    Q_OBJECT
  public:
    explicit IPlayerState(BioTracker3Player *player, IModel *textureObjectModel,
                          std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);
    enum PLAYER_STATES {STATE_INITIAL, STATE_INITIAL_STREAM, STATE_PLAY, STATE_STEP_FORW,
                        STATE_STEP_BACK, STATE_PAUSE, STATE_WAIT
                       };
    Q_ENUM(PLAYER_STATES)


  public Q_SLOTS:
    void changeImageStream(std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    virtual void operate();

  Q_SIGNALS:
    void emitStateOfPlay(bool);
    void emitStateOfStepForward(bool);
    void emitStateOfStepBackward(bool);
    void emitStateOfStop(bool);
    void emitStateOfPause(bool);

    void emitNextState(IPlayerState::PLAYER_STATES);
    void emitOperationDone();

  protected:
    BioTracker3Player *m_Player;

    BioTracker::Core::BioTracker3TextureObject *m_TextureObjectModel;

    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> m_ImageStream;
};

#endif // IPLAYERSTATE_H
