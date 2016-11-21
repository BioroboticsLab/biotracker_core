#include "ControllerGraphicScene.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "View/GraphicsView.h"
#include "Model/null_Model.h"

ControllerGraphicScene::ControllerGraphicScene(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerGraphicScene::addGraphicsItem(QGraphicsItem *item)
{
    GraphicsView *gview = dynamic_cast <GraphicsView *> (m_View);
    gview->addGraphicsItem(item);
}

void ControllerGraphicScene::addTextureObject(QGraphicsItem *item)
{
    GraphicsView *gview = dynamic_cast <GraphicsView *> (m_View);
    gview->addPixmapItem(item);
}

void ControllerGraphicScene::createModel()
{
    m_Element = new TrackedElement(this, QString("myElement"));
    m_NullModel = new null_Model();
}

void ControllerGraphicScene::createView()
{
    m_View = new GraphicsView(0, this, m_NullModel);
    m_ElementView = new TrackedElementView(0, this, m_Element);

    addGraphicsItem(m_ElementView);
}

void ControllerGraphicScene::connectModelController()
{

}

void ControllerGraphicScene::connectController()
{
        QPointer< IController > ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
        QPointer< BioTracker3MainWindow > mainWin = dynamic_cast<BioTracker3MainWindow *>(ctrM->getView());
        mainWin->addVideoView(m_View);
}
