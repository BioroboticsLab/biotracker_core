#include "IModelTrackedTrajectory.h"
#include "qdebug.h"

int IModelTrackedTrajectory::nextID = -1;

IModelTrackedTrajectory::IModelTrackedTrajectory(QObject *parent) :
	IModelTrackedComponent(parent)
{
	_id = ++nextID;
	qDebug() << "new traj:" << _id;
}

void IModelTrackedTrajectory::operate()
{
	// please implement this method in an TrackedObject class

}
