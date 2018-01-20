#ifndef CONTROLLERCOMMANDS_H
#define CONTROLLERCOMMANDS_H

#pragma once
#include "Interfaces/IController/IController.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Model/UndoCommands/TrackCommands.h"
#include "QUndoStack"
#include "QUndoView"


class ControllerCommands : public IController {
	Q_OBJECT
	public:
		ControllerCommands(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
		virtual ~ControllerCommands();

	signals:
		// signal to ctrPlugin to remove trajectory
		void emitRemoveTrajectory(IModelTrackedTrajectory* trajectory);

		void emitRemoveTrajectoryId(int id);

		// signal to ctrPlugin to remove track entity
		void emitRemoveTrackEntity(IModelTrackedTrajectory* trajectory, uint frameNumber);

		// signal to ctrPlugin to add trajectory
		void emitAddTrajectory(QPoint pos);

		void emitValidateTrajectory(int id);

		void emitValidateEntity(IModelTrackedTrajectory* trajectory, uint frameNumber);

		void emitMoveElement(IModelTrackedTrajectory* trajectory, uint frameNumber, QPoint pos, int toMove);

		void emitSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

		void emitToggleFixTrack(IModelTrackedTrajectory* trajectory, bool toggle);

		void emitEntityRotation(IModelTrackedTrajectory* trajectory, double angleDeg, uint frameNumber);


	public slots:
		void receiveAddTrackCommand(QPoint pos, int id);
		void receiveRemoveTrackCommand(IModelTrackedTrajectory* traj);
		void receiveRemoveTrackEntityCommand(IModelTrackedTrajectory* traj, uint frameNumber);
		void receiveMoveElementCommand(IModelTrackedTrajectory* traj , QPoint oldPos, QPoint newPos, uint frameNumber, int toMove);
		void receiveSwapIdCommand(IModelTrackedTrajectory* traj0, IModelTrackedTrajectory* traj1);
		void receiveFixTrackCommand(IModelTrackedTrajectory* traj, bool toggle);
		void receiveEntityRotation(IModelTrackedTrajectory* traj, double oldAngleDeg, double newAngleDeg, uint frameNumber);

		void receiveUndo();
		void receiveRedo();
		void receiveClear();
		void receiveShowActionList(); 

	// IController interface
	public:
		void connectControllerToController() override;

	protected:
		void createModel() override;
		void createView() override;
		void connectModelToController() override;
	//member
	private:
		QUndoStack* _undoStack;
		QUndoView* _undoView;
};

#endif // CONTROLLERCOMMANDS_H