#ifndef CONTROLLERCONTEXT_H
#define CONTROLLERCONTEXT_H

#include <QObject>

#include "Interfaces/IController/icontroller.h"
#include "QPointer"


class ControllerContext
{
public:
    ControllerContext();

    void executeBioTarckerController();

    void setBioTrackerController(IController *controller);
    IController *getBioTrackerController();

private:
    QPointer<IController> m_Controller;
};

#endif // CONTROLLERCONTEXT_H
