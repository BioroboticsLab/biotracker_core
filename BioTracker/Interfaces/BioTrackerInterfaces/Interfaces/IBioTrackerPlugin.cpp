#include "IBioTrackerPlugin.h"

IView *IBioTrackerPlugin::getTrackerParameterWidget()  { return nullptr; };
IView *IBioTrackerPlugin::getTrackerElementsWidget() { return nullptr; };
IModel *IBioTrackerPlugin::getTrackerComponentModel() { return nullptr; };
void IBioTrackerPlugin::sendCorePermissions() { return; };
IModelTrackedComponentFactory *IBioTrackerPlugin::getComponentFactory() { return nullptr; };
void IBioTrackerPlugin::connectInterfaces() { return; };
void IBioTrackerPlugin::receiveAreaDescriptor(IModelAreaDescriptor *areaDescr) { return; };