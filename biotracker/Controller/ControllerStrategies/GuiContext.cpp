#include "GuiContext.h"
#include "Controller/ControllerMainWindow.h"
#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "Model/BioTracker3Player.h"
#include "Model/BioTracker3TextureObject.h"

GuiContext::GuiContext(QObject *parent) :
    IBioTrackerContext(parent)
{
    m_ControllersMap.insert("MainWindowController", m_MainWindowController = new ControllerMainWindow(0, this));
    m_ControllersMap.insert("PlayerController", m_PlayerController = new ControllerPlayer(0, this));
    m_ControllersMap.insert("TextureObjectController", m_TextureObjectController = new ControllerTextureObject(0, this));
}

void GuiContext::createAppController()
{
    m_MainWindowController->createComponents();
    m_PlayerController->createComponents();
    m_TextureObjectController->createComponents();


}

void GuiContext::connectController()
{
    // comment
    m_PlayerController->connectViewToMainWindow(m_MainWindowController);
    // comment
    m_TextureObjectController->connectViewToMainWindow(m_MainWindowController);
    // comment
    m_TextureObjectController->connectToOtherController(m_PlayerController);



}
