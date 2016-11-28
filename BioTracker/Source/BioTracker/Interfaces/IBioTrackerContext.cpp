#include "IBioTrackerContext.h"
#include "Interfaces/IController/icontroller.h"

#include "Interfaces/IBioTrackerTrackingInterface.h"
#include "QDebug"


IBioTrackerContext::IBioTrackerContext(QObject *parent) :
    QObject(parent)
{

}

void IBioTrackerContext::createApplication()
{
    createAppController();
    connectController();
}

bool IBioTrackerContext::loadBioTrackerPlugin(QString str)
{
    bool x = QLibrary::isLibrary(str);
    m_PluginLoader = new QPluginLoader(str);

    QString as = m_PluginLoader->errorString();

    QObject *plugin = m_PluginLoader->instance();
    QString sb = m_PluginLoader->errorString();
    qDebug() << sb;

    if (plugin) {
        m_PluginController = qobject_cast<IBioTrackerTrackingInterface *>(plugin);
        if (m_PluginController) {
            m_PluginController->addBioTrackerContext(this);
            m_PluginController->createComponents();
            m_PluginController->connectComponents();
            return true;
        }
    }

    return false;
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
