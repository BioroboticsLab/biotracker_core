#include "IBioTrackerContext.h"
#include "Interfaces/IController/icontroller.h"


IBioTrackerContext::IBioTrackerContext(QObject *parent) :
    QObject(parent)
{

}

void IBioTrackerContext::createApplication()
{
    createAppController();
    connectController();
}

void IBioTrackerContext::createAppController()
{

}

IController *IBioTrackerContext::requestController(ENUMS::CONTROLLERTYPE ctrtype)
{
    return m_ControllersMap.value(ctrtype);
}

void IBioTrackerContext::connectController()
{

}

void IBioTrackerContext::addController(IController *ctr)
{
    m_ControllersMap.insert(ctr->getControllerType(), ctr);
}
