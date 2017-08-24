#include "ControllerTrackedComponent.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackingRectElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "View/TrackedElementView.h"

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
		t->setId(i);
		TrackedElement *e = new TrackedElement(t, "n.a.", i);
		e->setId(i);
		t->add(e);
		all->add(t);
	}
}

void ControllerTrackedComponent::createModel()
{
	TrackedTrajectory *t = new TrackedTrajectory(this, "All");
	
	//Add default trajectories
	createTrajectories(2, t);

	//Add rect corners for rectification
	TrackingRectElement *c1 = new TrackingRectElement(this, "", 0);
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
	t->add(c1);
	m_Model = t;
}


IView *ControllerTrackedComponent::getTrackingElementsWidget()
{
	return m_View;
}