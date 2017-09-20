#include "ControllerTrackedComponent.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "View/TrackedElementView.h"
#include "ControllerTrackingAlgorithm.h"

namespace sampleTracker {
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

	void ControllerTrackedComponent::createModel()
	{
		TrackedTrajectory *t = new TrackedTrajectory(this, "All");
		m_Model = t;
	}


	IView *ControllerTrackedComponent::getTrackingElementsWidget()
	{
		return m_View;
	}
}