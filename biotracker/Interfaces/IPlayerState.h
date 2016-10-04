#ifndef IPLAYERSTATE_H
#define IPLAYERSTATE_H

#include <QObject>

#include "Interfaces/imodel.h"

#include "BioTracker3ImageStream.h"
#include "BioTracker3TextureObject.h"

class BioTracker3Player;
class IPlayerState : public IModel {
    Q_OBJECT
  public:
    explicit IPlayerState(BioTracker3Player *player, IModel *textureObjectModel,
                          std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);
    enum PLAYER_STATES {STATE_INITIAL, STATE_VIDEO_INITIAL, STATE_PLAY, STATE_VIDEO_STEPNEXT};
    Q_ENUM(PLAYER_STATES)

    void changeImageStream(std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

  public Q_SLOTS:
    virtual void doIt();

  public:
    virtual bool stateOfPlay();
    virtual bool stateOfStepForward();
    virtual bool stateOfRew();
    virtual bool stateOfStop();

  Q_SIGNALS:
    void emitNextState(IPlayerState::PLAYER_STATES);

  protected:
    BioTracker3Player *m_Player;

    BioTracker::Core::BioTracker3TextureObject *m_TextureObjectModel;

    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> m_ImageStream;
};

#endif // IPLAYERSTATE_H
