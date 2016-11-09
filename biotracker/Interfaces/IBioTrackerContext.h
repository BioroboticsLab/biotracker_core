#ifndef IBIOTRACKERCONTEXT_H
#define IBIOTRACKERCONTEXT_H

#include <QObject>
#include "QMap"
#include "QString"

class IController;
class IBioTrackerContext : public QObject
{
    Q_OBJECT
public:
    IBioTrackerContext(QObject *parent = 0);

    void createApplication();

    IController * getController(QString ctrName);

protected:
    virtual void createAppController();
    virtual void connectController();
protected:
    QMap<QString, IController *> m_ControllersMap;


};

#endif // IBIOTRACKERCONTEXT_H
