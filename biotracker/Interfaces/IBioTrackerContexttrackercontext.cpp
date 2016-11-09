#include "IBioTrackerContext.h"
#include "Interfaces/icontroller.h"


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

IController *IBioTrackerContext::getController(QString ctrName)
{
    IController *ctr = 0;
    if (m_ControllersMap.contains(ctrName)) {
        ctr = m_ControllersMap.value(ctrName);
    }

    return ctr;
}

void IBioTrackerContext::connectController()
{

}
