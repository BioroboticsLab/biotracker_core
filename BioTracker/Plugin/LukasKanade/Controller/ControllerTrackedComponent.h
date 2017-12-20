#ifndef CONTROLLERTRACKEDCOMPONENT_H
#define CONTROLLERTRACKEDCOMPONENT_H

#include "Interfaces/IController/IController.h"
#include "QPoint"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"

class ControllerTrackedComponent : public IController
{
	Q_OBJECT
public:
	ControllerTrackedComponent(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::COMPONENT);

	IView *getTrackingElementsWidget();
public Q_SLOTS:
		void receiveAddTrajectory(QPoint position);
		void receiveRemoveTrajectory(IModelTrackedTrajectory* trajectory);
		void receiveMoveElement(IModelTrackedTrajectory* trajectory, QPoint position);
		/* TODO Swaps ID's of last elements (-> swap elements) or swap ID's of trajectories and all of its elements?
		*/
		void receiveSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);
		void receiveCurrentFrameNumber(uint framenumber);

	// IController interface
protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;

	//members
	int m_currentFrameNumber;
};

#endif // CONTROLLERTRACKEDCOMPONENT_H
