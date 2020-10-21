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
#include "Controller/ControllerAreaDescriptor.h"
#include "Controller/ControllerCoreParameter.h"
#include "Controller/ControllerCommands.h"
#include "Controller/ControllerNotifications.h"
#include "QPointer"

#include "QDebug"


GuiContext::GuiContext(QObject *parent, Config *cfg) :
    IBioTrackerContext(parent)
{
	_cfg = cfg;
    QPointer< IController > MainWindowController = new ControllerMainWindow(this, this, ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< IController > PlayerController = new ControllerPlayer(this, this, ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer< IController > TextureObjectController = new ControllerTextureObject(this, this, ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    QPointer< IController > GraphicsViewController = new ControllerGraphicScene(this, this, ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	QPointer< IController > TrackedComponentCoreController = new ControllerTrackedComponentCore(this, this, ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
	QPointer< IController > DataExportController = new ControllerDataExporter(this, this, ENUMS::CONTROLLERTYPE::DATAEXPORT);
	QPointer< IController > PluginController = new ControllerPlugin(this, this, ENUMS::CONTROLLERTYPE::PLUGIN);
	QPointer< IController > AnnotationsController = new ControllerAnnotations(this, this, ENUMS::CONTROLLERTYPE::ANNOTATIONS);
	QPointer< IController > AreaDescriptorController = new ControllerAreaDescriptor(this, this, ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);
	QPointer< IController > CoreParameterController = new ControllerCoreParameter(this, this, ENUMS::CONTROLLERTYPE::COREPARAMETER);
	QPointer< IController > CommandsController = new ControllerCommands(this, this, ENUMS::CONTROLLERTYPE::COMMANDS);
	QPointer< IController > NotificationController = new ControllerNotifications(this, this, ENUMS::CONTROLLERTYPE::NOTIFICATION);

    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::MAINWINDOW, MainWindowController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::PLAYER, PlayerController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT, TextureObjectController);
    m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW, GraphicsViewController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE, TrackedComponentCoreController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::DATAEXPORT, DataExportController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::PLUGIN, PluginController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::ANNOTATIONS, AnnotationsController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::AREADESCRIPTOR, AreaDescriptorController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::COREPARAMETER, CoreParameterController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::COMMANDS, CommandsController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::NOTIFICATION, NotificationController);

    for (auto x: m_ControllersMap){
        auto c = static_cast<IControllerCfg*>(x);
        c->setConfig(_cfg);
    }
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
    //activate tracking if autotracking activated in cfg and autoplay if in cfg 
    ControllerMainWindow *contMW = static_cast<ControllerMainWindow*>(m_ControllersMap.value(ENUMS::CONTROLLERTYPE::MAINWINDOW));
    ControllerPlayer *contP = static_cast<ControllerPlayer*>(m_ControllersMap.value(ENUMS::CONTROLLERTYPE::PLAYER));
    ControllerPlugin *contPlg = static_cast<ControllerPlugin*>(m_ControllersMap.value(ENUMS::CONTROLLERTYPE::PLUGIN));

    if (_cfg->AutoTrack != -1){
        contMW->activeTracking();

        // add as many tracks as set in CLI for autotracking
        qDebug() << "CORE: CtrPlugin: Adding " << _cfg->AutoTrack << " tracks ";
        for(int i = 0; i < _cfg->AutoTrack; i++){
            contPlg->emitAddTrajectory(QPoint(0,0));
        }

        if(_cfg->AutoPlay){
            contP->play();
        }
    }
    else if (_cfg->AutoTrack==-1 && _cfg->AutoPlay){
        contP->play();
    }
}

void GuiContext::exit() {

    QString cfgLoc = _cfg->CfgCustomLocation.isEmpty()?Config::configLocation:_cfg->CfgCustomLocation;
    _cfg->save(cfgLoc, "config.ini");
    QMap<ENUMS::CONTROLLERTYPE, IController *>::iterator i;
    for (i = m_ControllersMap.begin(); i != m_ControllersMap.end(); ++i)
    {
        i.value()->cleanup();
    }
    
    for (auto&& i : m_ControllersMap)
    {
        delete i;
    }

    qApp->exit();
	std::exit(0);
}
