#include "ControllerTrackedComponent.h"
#include "TrackedComponents/TrackedElement.h"
#include "TrackedComponents/TrackedTrajectory.h"
#include "View/TrackedElementView.h"
#include "qdebug.h"
#include "qmath.h"

ControllerTrackedComponent::ControllerTrackedComponent(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
	m_currentFrameNumber = 0;
}

void ControllerTrackedComponent::createView()
{
	m_View = new TrackedElementView(0, this, m_Model);
}

void ControllerTrackedComponent::connectModelToController()
{

}

void ControllerTrackedComponent::connectControllerToController()
{
}

void createTrajectories(int count, TrackedTrajectory* all) {
	//This should be done using a factory, right?
	for (int i = 0; i < count; i++) {
		TrackedTrajectory *t = new TrackedTrajectory();
		t->setValid(true);
		TrackedElement *e = new TrackedElement(t, "n.a.", t->getId());
		t->add(e);
		all->add(t, i);
	}
}

void ControllerTrackedComponent::createModel()
{
	TrackedTrajectory *t = new TrackedTrajectory(this, "All");

	m_Model = t;
}


IView *ControllerTrackedComponent::getTrackingElementsWidget()
{
	return m_View;
}

void ControllerTrackedComponent::receiveRemoveTrajectory(IModelTrackedTrajectory * trajectory)
{
	trajectory->setValid(false);
	qDebug() << "trajectory" << trajectory->getId() << "set invalid";
}

void ControllerTrackedComponent::receiveRemoveTrajectoryId(int id)
{
	TrackedTrajectory* allTraj = qobject_cast<TrackedTrajectory*>(m_Model);
	if (allTraj) {
		IModelTrackedComponent* traj = allTraj->getChild(id - 1);
		traj->setValid(false);
		qDebug() << "track" << id << "set invalid";
	}
}

void ControllerTrackedComponent::receiveValidateTrajectory(int id)
{
	TrackedTrajectory* allTraj = qobject_cast<TrackedTrajectory*>(m_Model);
	if (allTraj) {
		IModelTrackedComponent* traj = allTraj->getChild(id - 1);
		traj->setValid(true);
		qDebug() << "track" << id << "validated";
	}
}

void ControllerTrackedComponent::receiveValidateEntity(IModelTrackedTrajectory * trajectory, uint frameNumber)
{
	trajectory->getChild(frameNumber)->setValid(true);
	qDebug() << "track " << trajectory->getId() << " entity #" << frameNumber << "set valid";
}

void ControllerTrackedComponent::receiveRemoveTrackEntity(IModelTrackedTrajectory * trajectory, uint frameNumber)
{
	trajectory->getChild(frameNumber)->setValid(false);
	qDebug() << "track " << trajectory->getId() << " entity #" << frameNumber << "set invalid";
}

void ControllerTrackedComponent::receiveAddTrajectory(QPoint position)
{
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	TrackedTrajectory* newTraj = new TrackedTrajectory();
	TrackedElement* firstElem = new TrackedElement(newTraj, "n.a.", newTraj->getId());
	firstElem->setX(position.x());
	firstElem->setY(position.y());
	firstElem->setTime(start);
	firstElem->setValid(true);
	newTraj->add(firstElem, m_currentFrameNumber);
	TrackedTrajectory* allTraj = qobject_cast<TrackedTrajectory*>(m_Model);
	if (allTraj) {
		allTraj->add(newTraj);
		qDebug() << "trajectory added at" << firstElem->getX() << "," << firstElem->getY();
	}
}

void ControllerTrackedComponent::receiveMoveElement(IModelTrackedTrajectory* trajectory, uint frameNumber, QPoint position)
{
	TrackedTrajectory* traj = dynamic_cast<TrackedTrajectory*>(trajectory);
	// dont't move starter dummies and main trajectory (id's: 0,1,2)!!
    if (!(traj->getId() == 0) && frameNumber >= 0) {
		TrackedElement* element = dynamic_cast<TrackedElement*>(traj->getChild(frameNumber));
		//TODO setX, setY do not work correctly as pose not yet accessible
        if (element) {
            element->setX(position.x());
            element->setY(position.y());
            qDebug() << "plugin-pos:" << position;
        }
        else {
            qDebug() << "Not found (moved and deleted?):" << position;
        }
	}
}

void ControllerTrackedComponent::receiveSwapIds(IModelTrackedTrajectory * trajectory0, IModelTrackedTrajectory * trajectory1)
{
	//TODO do this, not just nothing
	TrackedTrajectory* traj0 = dynamic_cast<TrackedTrajectory*>(trajectory0);
	TrackedTrajectory* traj1 = dynamic_cast<TrackedTrajectory*>(trajectory1);

	// dont't move main trajectory (id's: 0)!!
	if (!(traj0->getId() == 0) && !(traj1->getId() == 0)) {
		int traj0Id = traj0->getId();
		int traj1Id = traj1->getId();

		traj0->setId(traj1Id);
		traj1->setId(traj0Id);

		qDebug() << "Swap IDs " << traj0Id << "and " << traj1Id;
	}
}

void ControllerTrackedComponent::receiveToggleFixTrack(IModelTrackedTrajectory * trajectory, bool toggle)
{
	trajectory->setFixed(toggle);
}

void ControllerTrackedComponent::receiveEntityRotation(IModelTrackedTrajectory * trajectory, double angle, uint frameNumber)
{
	TrackedTrajectory* traj = dynamic_cast<TrackedTrajectory*>(trajectory);
	if (traj) {
		IModelTrackedPoint* pointLike = dynamic_cast<IModelTrackedPoint*>(traj->getChild(frameNumber));
		if (pointLike) {
			pointLike->setDeg(float(angle));
			pointLike->setRad(float(qDegreesToRadians(angle)));
		}
	}
}

void ControllerTrackedComponent::receiveCurrentFrameNumber(uint framenumber)
{
	m_currentFrameNumber = (int)framenumber;
}
