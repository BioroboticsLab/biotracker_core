#ifndef CONTROLLERTRACKINGALGORITHM_H
#define CONTROLLERTRACKINGALGORITHM_H

#include "Interfaces/icontroller.h"

class ControllerTrackingAlgorithm : public IController
{
    Q_OBJECT
public:
    ControllerTrackingAlgorithm();

    // IController interface
public:
    void connectViewToMainWindow(IController *controller) override;
    void connectToOtherController(IController *controller) override;

protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectModelView() override;
};

#endif // CONTROLLERTRACKINGALGORITHM_H
