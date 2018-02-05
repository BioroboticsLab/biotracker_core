#ifndef TRACKEDCOMPONENTFACTORY_H
#define TRACKEDCOMPONENTFACTORY_H

#include "Interfaces/IModel/IModelTrackedComponentFactory.h"

class TrackedComponentFactory : public IModelTrackedComponentFactory
{
    Q_OBJECT
public:
    TrackedComponentFactory();
    ~TrackedComponentFactory();

    QList<QString> getElementTypes() override;

    // ITrackedComponentFactory interface
protected:
    IModelTrackedComponent *createTrackedElement(QString name) override;
	IModelTrackedComponent *createTrackedObject(QString name) override;
	IModelTrackedComponent *createTrackedTrajectory(QString name) override;
};

#endif // TRACKEDOBJECTFACTORY_H
