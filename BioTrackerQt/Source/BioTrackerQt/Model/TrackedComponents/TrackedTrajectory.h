#ifndef TRACKEDOTRAJECTORY_H
#define TRACKEDOTRAJECTORY_H

#include "Interfaces/IModel/ITrackedTrajectory.h"
#include "QList"
#include "QString"


class TrackedTrajectory : public ITrackedTrajectory {
    Q_OBJECT

  public:
    TrackedTrajectory(QObject *parent = 0, QString name = "n.a.");

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
    QList<ITrackedComponent*> m_TrackedComponents;

    QString name;
};

#endif // TRACKEDOTRAJECTORY_H
