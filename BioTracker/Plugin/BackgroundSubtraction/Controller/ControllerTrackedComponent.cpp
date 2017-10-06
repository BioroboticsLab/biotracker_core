#include "ControllerTrackedComponent.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackingRectElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "View/TrackedElementView.h"
#include "qdebug.h"

ControllerTrackedComponent::ControllerTrackedComponent(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{

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
		//t->setId(i);
		TrackedElement *e = new TrackedElement(t, "n.a.", t->getId());
		t->add(e);
		all->add(t, i);
	}
}

void ControllerTrackedComponent::createModel()
{
	TrackedTrajectory *t = new TrackedTrajectory(this, "All");
	
	//Add default trajectories
	createTrajectories(2, t);

	//Add rect corners for rectification
/*	TrackingRectElement *c1 = new TrackingRectElement(this, "", 0);
	c1->setX(100);
	c1->setY(100);
	t->add(c1);
	c1 = new TrackingRectElement(this, "", 1);
	c1->setX(100);
	c1->setY(2040);
	t->add(c1);
	c1 = new TrackingRectElement(this, "", 2);
	c1->setX(2040);
	c1->setY(2040);
	t->add(c1);
	c1 = new TrackingRectElement(this, "", 3);
	c1->setX(2040);
	c1->setY(100);
	t->add(c1);*/
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

void ControllerTrackedComponent::receiveAddTrajectory(QPoint position)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	TrackedTrajectory* newTraj = new TrackedTrajectory();
	TrackedElement* firstElem = new TrackedElement(newTraj, "n.a.", newTraj->getId());
	firstElem->setX(position.x());
	firstElem->setY(position.y());
	firstElem->setTime(start);
	newTraj->add(firstElem, m_currentFrameNumber);
	TrackedTrajectory* allTraj = qobject_cast<TrackedTrajectory*>(m_Model);
	if (allTraj) {
		allTraj->add(newTraj);
		qDebug() << "trajectory added at" << firstElem->getX() << "," << firstElem->getY();
	}
}

void ControllerTrackedComponent::receiveMoveElement(IModelTrackedTrajectory* trajectory, QPoint position)
{
	TrackedTrajectory* traj = dynamic_cast<TrackedTrajectory*>(trajectory);
	// dont't move starter dummies and main trajectory (id's: 0,1,2)!!
	if (!traj->getId() <= 2) {
		TrackedElement* element = dynamic_cast<TrackedElement*>(traj->getChild(m_currentFrameNumber));
		//TODO setX, setY do not work correctly as pose not yet accessible
		element->setX(position.x());
		element->setY(position.y());
		qDebug() << "plugin-pos:" << position;
	}
}

void ControllerTrackedComponent::receiveSwapIds(IModelTrackedTrajectory * trajectory0, IModelTrackedTrajectory * trajectory1)
{
	//TODO do this, not just nothing
	TrackedTrajectory* traj0 = dynamic_cast<TrackedTrajectory*>(trajectory0);
	TrackedTrajectory* traj1 = dynamic_cast<TrackedTrajectory*>(trajectory1);

	// dont't move starter dummies and main trajectory (id's: 0,1,2)!!
	if (!traj0->getId() <= 2 && !traj1->getId() <= 2) {
		int traj0Id = traj0->getId();
		int traj1Id = traj1->getId();

		qDebug() << "TODO: Swap IDs " << traj0Id << "and " << traj1Id;
	}
}

void ControllerTrackedComponent::receiveCurrentFrameNumber(uint framenumber)
{
	m_currentFrameNumber = (int)framenumber;
}
