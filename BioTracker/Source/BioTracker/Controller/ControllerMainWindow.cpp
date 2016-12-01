#include "ControllerMainWindow.h"
#include "View/BioTracker3MainWindow.h"
#include "Model/null_Model.h"
//#include "Controller/ControllerStrategies/MainGUIApplication.h"
#include "Controller/ControllerPlayer.h"
#include "GuiContext.h"

#include "QPluginLoader"


ControllerMainWindow::ControllerMainWindow(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerMainWindow::loadVideo(QString str)
{
    IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer *>(ctr)->loadVideoStream(str);
}

void ControllerMainWindow::loadTracker(QString str)
{
    qobject_cast<GuiContext *>(m_BioTrackerContext)->loadBioTrackerPlugin(str);

}

void ControllerMainWindow::loadPictures(std::vector<boost::filesystem::path> files)
{
    IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer *>(ctr)->loadPictures(files);
}

void ControllerMainWindow::loadCameraDevice(int i)
{
    IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer *>(ctr)->loadCameraDevice(i);
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

void ControllerMainWindow::connectController()
{

}
