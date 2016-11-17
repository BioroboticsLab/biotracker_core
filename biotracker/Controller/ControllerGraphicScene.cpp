#include "ControllerGraphicScene.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "../biotracker_gui/biotracker/View/TrackedElementView.h"
#include "../biotracker_gui/biotracker/View/GraphicsView.h"
#include "Model/null_Model.h"

ControllerGraphicScene::ControllerGraphicScene(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerGraphicScene::createModel()
{
    m_Element = new TrackedElement(0, QString("myElement"));
    m_NullModel = new null_Model();
}

void ControllerGraphicScene::createView()
{
    m_View = new GraphicsView(0, this, m_NullModel);
    m_ElementView = new TrackedElementView(0, this, m_Element);
}

void ControllerGraphicScene::connectModelController()
{

}

void ControllerGraphicScene::connectController()
{

}
