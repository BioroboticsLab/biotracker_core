#ifndef TRACKEDOBJECT_H
#define TRACKEDOBJECT_H

#include "Interfaces/ITrackedObject.h"
#include "QList"
#include "QString"


class TrackedObject : public ITrackedObject {
    Q_OBJECT

  public:
    TrackedObject(QObject *parent = 0, QString name = "n.a.");

    // ITrackedComponent interface
public:
    void operate();

    // ITrackedObject interface
public:
    void add(ITrackedComponent *comp) override;
    void remove(ITrackedComponent *comp) override;
    ITrackedComponent *getChild(int index) override;
    int numberOfChildrean();

private:
    QList<ITrackedComponent*> m_TrackedObject;

    QString name;
};

#endif // TRACKEDOBJECT_H
