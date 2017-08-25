#pragma once

#include "Interfaces/IController/IController.h"

class ControllerAreaDescriptor : public IController
{
	Q_OBJECT
public:
	ControllerAreaDescriptor(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);

	//IView *getTrackingElementsWidget();

private slots:
	void trackingAreaType(int v);

	// IController interface
protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;
};