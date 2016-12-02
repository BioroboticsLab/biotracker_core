#ifndef CONTROLLERBIOTRACKER_H
#define CONTROLLERBIOTRACKER_H

#include "Interfaces/IController/icontroller.h"



class ControllerBioTracker : public IController
{
public:
    ControllerBioTracker();

    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectController() override;
};

#endif // CONTROLLERBIOTRACKER_H
