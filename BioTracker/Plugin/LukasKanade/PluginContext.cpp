#include "PluginContext.h"

#include "Controller/ControllerTrackingAlgorithm.h"
#include "Controller/ControllerTrackedComponent.h"

PluginContext::PluginContext(QObject *parent) :
	IBioTrackerContext(parent)
{
	QPointer< IController > ComponentController = new ControllerTrackedComponent(this, this, ENUMS::CONTROLLERTYPE::COMPONENT);
	QPointer< IController > TrackingController = new ControllerTrackingAlgorithm(this, this, ENUMS::CONTROLLERTYPE::TRACKING);

	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::COMPONENT, ComponentController);
	m_ControllersMap.insert(ENUMS::CONTROLLERTYPE::TRACKING, TrackingController);
}

void PluginContext::createAppController()
{
	QMap<ENUMS::CONTROLLERTYPE, IController *>::iterator i;
	for (i = m_ControllersMap.begin(); i != m_ControllersMap.end(); ++i)
	{
		i.value()->createComponents();
	}
}

void PluginContext::connectController()
{
	QMap<ENUMS::CONTROLLERTYPE, IController *>::iterator i;
	for (i = m_ControllersMap.begin(); i != m_ControllersMap.end(); ++i)
	{
		i.value()->connectComponents();
	}
}


