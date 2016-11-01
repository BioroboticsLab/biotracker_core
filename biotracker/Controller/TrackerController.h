#ifndef TRACKERCONTROLLER_H
#define TRACKERCONTROLLER_H

#include "QThread"

#include "Interfaces/icontroller.h"
#include "Interfaces/ITrackingAlgorithm.h"
#include "Interfaces/ITrackedComponent.h"
#include "Interfaces/ITrackedComponentFactory.h"


class TrackerController : public IController {
  public:
    TrackerController();

    // IController interface
public:
    void createApplication();

    void addPlayerController(IController *controller);

private:
    ITrackingAlgorithm *m_TrackingAlgorithm;
    ITrackedComponent *m_TrackedComponent;
    ITrackedComponentFactory *m_TrackedComponentFactory;

    IController *m_PlayerController;

    QThread m_TrackerThread;

    IView *m_TrackedImageView;
    IModel *m_TrackedTexture;
};

#endif // TRACKERCONTROLLER_H
