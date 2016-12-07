#ifndef TRACKEDOTRAJECTORY_H
#define TRACKEDOTRAJECTORY_H

#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "QList"
#include "QString"


class TrackedTrajectory : public IModelTrackedTrajectory {
    Q_OBJECT

  public:
    TrackedTrajectory(QObject *parent = 0, QString name = "n.a.");

    // ITrackedComponent interface
public:
    void operate();

    // ITrackedObject interface
public:
    void add(IModelTrackedComponent *comp) override;
    void remove(IModelTrackedComponent *comp) override;
    IModelTrackedComponent *getChild(int index) override;
    int numberOfChildrean();

private:
    QList<IModelTrackedComponent*> m_TrackedComponents;

    QString name;
};

#endif // TRACKEDOTRAJECTORY_H
