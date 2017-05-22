#ifndef TRACKEDOTRAJECTORY_H
#define TRACKEDOTRAJECTORY_H

#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "QList"
#include "QString"

/**
 * This class inherits from the IModelTrackedComponent class and is therefor part of the Composite Pattern.
 * This class represents the Composite class as it is stated by the GoF.
 * This class is responsibility for the handling of Leaf objects.
 * Internaly this class uses a QList for storing Leaf object.
 *
 * Objects of this class can have a QObject as parent and as it appears in this example have a name.
 */
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
	IModelTrackedComponent *getLastChild() override;
    int numberOfChildrean();

private:
    QList<IModelTrackedComponent*> m_TrackedComponents;

    QString name;
};

#endif // TRACKEDOTRAJECTORY_H
