#include "ControllerTrackedComponent.h"
#include "Model/TrackedComponents/TrackedElement.h"
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

void ControllerTrackedComponent::createModel()
{
    TrackedElement *elem = new TrackedElement(this, "Hallo");
    elem->setX(70);
    elem->setY(90);

    m_Model = elem;

}
