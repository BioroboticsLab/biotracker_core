#include "GuiContext.h"
#include "Controller/ControllerMainWindow.h"
#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "Controller/ControllerTrackingAlgorithm.h"
#include "Model/BioTracker3Player.h"
#include "Model/BioTracker3TextureObject.h"

GuiContext::GuiContext(QObject *parent) :
    IBioTrackerContext(parent)
{
    IController *MainWindowController = new ControllerMainWindow(0, this, ENUMS::CONTROLLERTYPE::MAINWINDOW);
    IController *PlayerController = new ControllerPlayer(0, this, ENUMS::CONTROLLERTYPE::PLAYER);
    IController *TextureObjectController = new ControllerTextureObject(0, this, ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    IController *TrackingAlgoController = new ControllerTrackingAlgorithm(0, this, ENUMS::CONTROLLERTYPE::TRACKING);

    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::MAINWINDOW, MainWindowController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::PLAYER, PlayerController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT, TextureObjectController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TRACKING, TrackingAlgoController);

}

void GuiContext::createAppController()
{
    m_ControllersMap.value(ENUMS::CONTROLLERTYPE::MAINWINDOW)->createComponents();
    m_ControllersMap.value(ENUMS::CONTROLLERTYPE::PLAYER)->createComponents();
    m_ControllersMap.value(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT)->createComponents();
    m_ControllersMap.value(ENUMS::CONTROLLERTYPE::TRACKING)->createComponents();



}

void GuiContext::connectController()
{


}
