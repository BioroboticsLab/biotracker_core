#ifndef IBIOTRACKERCONTEXT_H
#define IBIOTRACKERCONTEXT_H

#include <QObject>
#include "QMap"
#include "QString"
#include "Interfaces/ENUMS.h"

#include "QPluginLoader"
//#include "Interfaces/IBioTrackerTrackingInterface.h"

class IBioTrackerTrackingInterface;
class IController;
class IBioTrackerContext : public QObject
{
    Q_OBJECT
public:
    IBioTrackerContext(QObject *parent = 0);

    void createApplication();

    bool loadBioTrackerPlugin(QString str);

    IController * requestController(ENUMS::CONTROLLERTYPE ctrtype);

protected:
    virtual void createAppController();
    virtual void connectController();

    void addController(IController * ctr);
protected:
    QMap<ENUMS::CONTROLLERTYPE, IController *> m_ControllersMap;

    IBioTrackerTrackingInterface *m_PluginController;

    QPluginLoader *m_PluginLoader;


};

#endif // IBIOTRACKERCONTEXT_H
