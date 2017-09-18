#ifndef CONTROLLERTRACKEDCOMPONENT_H
#define CONTROLLERTRACKEDCOMPONENT_H

#include "Interfaces/IController/IController.h"
#include "QPoint"
#include "Interfaces\IModel\IModelTrackedTrajectory.h"

class ControllerTrackedComponent : public IController
{
	Q_OBJECT
public:
	ControllerTrackedComponent(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::COMPONENT);

	IView *getTrackingElementsWidget();
public Q_SLOTS:
		void receiveAddTrajectory(QPoint position);
		//TODO setup accessing by id
		void receiveRemoveTrajectory(IModelTrackedTrajectory* trajectory);
		//TODO move specific element or last element of trajectory? --> last child does not work when rewinding
		void receiveMoveElement(IModelTrackedTrajectory* trajectory, QPoint position);

		//void receiveCurrentFrameNumber(int framenumber);


	// IController interface
protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;

	//members
	int m_currentframe;
};

#endif // CONTROLLERTRACKEDCOMPONENT_H
