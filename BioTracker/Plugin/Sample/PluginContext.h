#pragma once
#include "Interfaces/IBioTrackerContext.h"
#include "Interfaces/IController/IController.h"

class PluginContext : public IBioTrackerContext
{
	Q_OBJECT

public:
	PluginContext(QObject *parent = 0);

	// IBioTrackerContext interface
protected:
	void createAppController() override;
	void connectController() override;
};

