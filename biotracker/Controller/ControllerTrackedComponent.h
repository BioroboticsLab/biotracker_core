#ifndef CONTROLLERTRACKEDCOMPONENT_H
#define CONTROLLERTRACKEDCOMPONENT_H

#include "Interfaces/icontroller.h"

class ControllerTrackedComponent : public IController
{
public:
    ControllerTrackedComponent();

    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectController() override;
};

#endif // CONTROLLERTRACKEDCOMPONENT_H
