#pragma once

#include "Interfaces/IController/IController.h"
#include "Interfaces/IBioTrackerPlugin.h"
#include "QPointer"
#include "QThread"
#include "Model/MediaPlayer.h"

class ControllerDataExporter : public IController {
	Q_OBJECT
public:
	ControllerDataExporter(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
	~ControllerDataExporter();

	// IController interface
public:
	void connectControllerToController() override;

	public Q_SLOTS:

protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;

private Q_SLOTS:
	void rcvPlayerParameters(playerParameters* parameters);
};

