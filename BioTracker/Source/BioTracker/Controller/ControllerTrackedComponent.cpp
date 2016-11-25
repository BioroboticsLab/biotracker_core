#include "ControllerTrackedComponent.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "View/TrackedElementView.h"
#include "Controller/ControllerTextureObject.h"

ControllerTrackedComponent::ControllerTrackedComponent(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerTrackedComponent::createView()
{
    m_View = new TrackedElementView(0, this, m_Model);
}

void ControllerTrackedComponent::connectModelController()
{

}

void ControllerTrackedComponent::connectController()
{
    IController *ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    ControllerTextureObject *texture = dynamic_cast<ControllerTextureObject *>(ctrM);
    texture->addTextureElementView(m_View);
}

void ControllerTrackedComponent::createModel()
{
    TrackedElement *elem = new TrackedElement(this, "Hallo");
    elem->setX(70);
    elem->setY(90);

    m_Model = elem;

}
