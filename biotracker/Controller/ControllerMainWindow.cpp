#include "ControllerMainWindow.h"
#include "../biotracker_gui/biotracker/View/BioTracker3MainWindow.h"
#include "Model/null_Model.h"
#include "Controller/ControllerStrategies/MainGUIApplication.h"
#include "Controller/ControllerPlayer.h"

ControllerMainWindow::ControllerMainWindow(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerMainWindow::loadVideo(QString str)
{
    IController *ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    dynamic_cast<ControllerPlayer *>(ctr)->loadImageStream(str);
}

void ControllerMainWindow::connectViewToMainWindow(IController *controller)
{
    m_partnerController = controller;
}

void ControllerMainWindow::createModel()
{
    // no model for MainWindow
    m_Model = new null_Model();
}

void ControllerMainWindow::createView()
{
    m_View = dynamic_cast<IView *> (new BioTracker3MainWindow(0, this, m_Model));


    static_cast<BioTracker3MainWindow *>(m_View)->show();
}

void ControllerMainWindow::connectModelController()
{

}

void ControllerMainWindow::connectModelView()
{

}
