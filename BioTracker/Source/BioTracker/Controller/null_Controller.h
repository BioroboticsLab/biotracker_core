#ifndef NULL_CONTROLLER_H
#define NULL_CONTROLLER_H

#include "Interfaces/IController/icontroller.h"
class null_Controller : public IController
{
public:
    null_Controller();

    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectController() override;
};

#endif // NULL_CONTROLLER_H
