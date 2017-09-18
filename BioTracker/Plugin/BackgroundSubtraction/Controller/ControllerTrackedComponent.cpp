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
		//e->setId(i);
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
}

void ControllerTrackedComponent::receiveAddTrajectory(QPoint position)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	// TODO traj gets new id but element gets only appended -> position = current framenumber
	TrackedTrajectory* newTraj = new TrackedTrajectory();
	TrackedElement* firstElem = new TrackedElement(newTraj, "n.a.", newTraj->getId());
	firstElem->setX(position.x());
	firstElem->setY(position.y());
	firstElem->setTime(start);
	newTraj->add(firstElem);
	TrackedTrajectory* allTraj = qobject_cast<TrackedTrajectory*>(m_Model);
	if (allTraj) {
		allTraj->add(newTraj);
	}
}

void ControllerTrackedComponent::receiveMoveElement(IModelTrackedTrajectory* trajectory, QPoint position)
{

	//TODO not last child but by id or last active child!
	TrackedTrajectory* traj = dynamic_cast<TrackedTrajectory*>(trajectory);
	TrackedElement* lastChild = dynamic_cast<TrackedElement*>(traj->getLastChild());
	lastChild->setX(position.x());
	lastChild->setY(position.y());
	qDebug() << "plugin-pos:" << position;
}
