#ifndef CONTROLLERTRACKEDCOMPONENTCORE_H
#define CONTROLLERTRACKEDCOMPONENTCORE_H

#include "Interfaces/IController/IController.h"

class ControllerTrackedComponentCore : public IController
{
	Q_OBJECT
	public:
		ControllerTrackedComponentCore(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);

		IView *getTrackingElementsWidgetCore();

		void addModel(IModel* model);

	public Q_SLOTS:
		//this slot gets triggered when the tracking is done, so the core-view updates
		void receiveTrackingOperationDone(uint framenumber);

		// IController interface
	protected:
		void createModel() override;
		void createView() override;
		void connectModelToController() override;
		void connectControllerToController() override;
};

#endif // CONTROLLERTRACKEDCOMPONENTCORE_H
