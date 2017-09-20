#include "GuiContext.h"
#include "Controller/ControllerMainWindow.h"
#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "Controller/ControllerPlugin.h"
#include "Controller/ControllerAnnotations.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"
#include "Model/TextureObject.h"
#include "Controller/ControllerGraphicScene.h"
#include "Controller/ControllerDataExporter.h"
//#include "Controller/ControllerAreaDescriptor.h"
#include "QPointer"

#include "QDebug"


GuiContext::GuiContext(QObject *parent) :
    IBioTrackerContext(parent)
{
    QPointer< IController > MainWindowController = new ControllerMainWindow(this, this, ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< IController > PlayerController = new ControllerPlayer(this, this, ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer< IController > TextureObjectController = new ControllerTextureObject(this, this, ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
//    QPointer< IController > TrackingAlgoController = new ControllerTrackingAlgorithm(this, this, ENUMS::CONTROLLERTYPE::TRACKING);
    QPointer< IController > GraphicsViewController = new ControllerGraphicScene(this, this, ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	QPointer< IController > TrackedComponentCoreController = new ControllerTrackedComponentCore(this, this, ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
	QPointer< IController > PluginController = new ControllerPlugin(this, this, ENUMS::CONTROLLERTYPE::PLUGIN);
	QPointer< IController > DataExportController = new ControllerDataExporter(this, this, ENUMS::CONTROLLERTYPE::DATAEXPORT);
	QPointer< IController > AnnotationsController = new ControllerAnnotations(this, this, ENUMS::CONTROLLERTYPE::ANNOTATIONS);
	
//	QPointer< IController > AreaDescriptorController = new ControllerAreaDescriptor(this, this, ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);



    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::MAINWINDOW, MainWindowController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::PLAYER, PlayerController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT, TextureObjectController);
//    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TRACKING, TrackingAlgoController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW, GraphicsViewController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE, TrackedComponentCoreController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::PLUGIN, PluginController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::DATAEXPORT, DataExportController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::ANNOTATIONS, AnnotationsController);
//	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::AREADESCRIPTOR, AreaDescriptorController);

}

void GuiContext::loadBioTrackerPlugin(QString str)
{
    qobject_cast<ControllerPlugin*> (m_ControllersMap.value(ENUMS::CONTROLLERTYPE::PLUGIN))->loadPluginFromFileName(str);
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
