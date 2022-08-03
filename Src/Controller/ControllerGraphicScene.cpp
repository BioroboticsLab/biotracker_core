#include "ControllerGraphicScene.h"
//#include "Model/TrackedComponents/TrackedElement.h"
#include "View/GraphicsView.h"
#include "Model/null_Model.h"
#include "View/MainWindow.h"

ControllerGraphicScene::ControllerGraphicScene(QObject*              parent,
                                               IBioTrackerContext*   context,
                                               ENUMS::CONTROLLERTYPE ctr)
: IControllerCfg(parent, context, ctr)
{
    m_Element      = NULL;
    m_GraphicsView = NULL;
}

void ControllerGraphicScene::addGraphicsItem(QGraphicsItem* item)
{
    GraphicsView* gview = dynamic_cast<GraphicsView*>(m_View);
    gview->addGraphicsItem(item);
}

void ControllerGraphicScene::addTextureObject(QGraphicsItem* item)
{
    GraphicsView* gview = dynamic_cast<GraphicsView*>(m_View);
    gview->addPixmapItem(item);
}

void ControllerGraphicScene::createModel()
{
    m_NullModel = new null_Model();
}

void ControllerGraphicScene::createView()
{
    m_View = new GraphicsView(0, this, m_NullModel);
}

void ControllerGraphicScene::connectModelToController()
{
    QObject::connect(this,
                     &ControllerGraphicScene::signalToggleAntialiasingFull,
                     dynamic_cast<GraphicsView*>(m_View),
                     &GraphicsView::receiveToggleAntialiasingFull);
}

void ControllerGraphicScene::connectControllerToController()
{
    IController* ctrM = m_BioTrackerContext->requestController(
        ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer<MainWindow> mainWin = dynamic_cast<MainWindow*>(ctrM->getView());
    mainWin->addVideoView(m_View);
}

void ControllerGraphicScene::receiveToggleAntialiasingFull(bool toggle)
{
    signalToggleAntialiasingFull(toggle);
}