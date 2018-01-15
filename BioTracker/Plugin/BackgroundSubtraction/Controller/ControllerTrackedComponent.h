#ifndef CONTROLLERTRACKEDCOMPONENT_H
#define CONTROLLERTRACKEDCOMPONENT_H

#include "Interfaces/IController/IController.h"
#include "QPoint"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Interfaces/IModel/IModelAreaDescriptor.h"

class ControllerTrackedComponent : public IController
{
	Q_OBJECT
public:
	ControllerTrackedComponent(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::COMPONENT);

	IView *getTrackingElementsWidget();
public Q_SLOTS:
		void receiveAddTrajectory(QPoint position);
		void receiveRemoveTrajectory(IModelTrackedTrajectory* trajectory);
		void receiveRemoveTrajectoryId(int id);
		void receiveRemoveTrackEntity(IModelTrackedTrajectory* trajectory, uint frameNumber);
		void receiveValidateTrajectory(int id);
		void receiveValidateEntity(IModelTrackedTrajectory* trajectory, uint frameNumber);
		void receiveMoveElement(IModelTrackedTrajectory* trajectory, uint frameNumber, QPoint position);
		/* TODO Swaps ID's of last elements (-> swap elements) or swap ID's of trajectories and all of its elements?
		*/
		void receiveSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);
		void receiveToggleFixTrack(IModelTrackedTrajectory* trajectory, bool toggle);
		void receiveEntityRotation(IModelTrackedTrajectory* trajectory, double angle, uint frameNumber);
		void receiveCurrentFrameNumber(uint framenumber);

		void receiveAreaDescriptorUpdate(IModelAreaDescriptor *areaDescr);

	// IController interface
protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;

	//members
	int m_currentFrameNumber;
	IModelAreaDescriptor* m_areaDescr;
};

#endif // CONTROLLERTRACKEDCOMPONENT_H
