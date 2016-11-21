#include "GuiContext.h"
#include "Controller/ControllerMainWindow.h"
#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "Controller/ControllerTrackingAlgorithm.h"
#include "Model/BioTracker3Player.h"
#include "Model/BioTracker3TextureObject.h"
#include "Controller/ControllerGraphicScene.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "QPointer"

GuiContext::GuiContext(QObject *parent) :
    IBioTrackerContext(parent)
{
    QPointer< IController > MainWindowController = new ControllerMainWindow(this, this, ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< IController > PlayerController = new ControllerPlayer(this, this, ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer< IController > TextureObjectController = new ControllerTextureObject(this, this, ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    QPointer< IController > TrackingAlgoController = new ControllerTrackingAlgorithm(this, this, ENUMS::CONTROLLERTYPE::TRACKING);
    QPointer< IController > GraphicsViewController = new ControllerGraphicScene(this, this, ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);

    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::MAINWINDOW, MainWindowController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::PLAYER, PlayerController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT, TextureObjectController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TRACKING, TrackingAlgoController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW, GraphicsViewController);

}

void GuiContext::createAppController()
{
    QMap<ENUMS::CONTROLLERTYPE, IController *>::iterator i;
    for (i = m_ControllersMap.begin(); i != m_ControllersMap.end(); ++i)
    {
        i.value()->createComponents();
    }
}

void GuiContext::connectController()
{
    QMap<ENUMS::CONTROLLERTYPE, IController *>::iterator i;
    for (i = m_ControllersMap.begin(); i != m_ControllersMap.end(); ++i)
    {
        i.value()->connectComponents();
    }
}
