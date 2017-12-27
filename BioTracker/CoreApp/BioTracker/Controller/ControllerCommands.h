#ifndef CONTROLLERCOMMANDS_H
#define CONTROLLERCOMMANDS_H

#pragma once
#include "Interfaces/IController/IController.h"
#include "Interfaces\IModel\IModelTrackedTrajectory.h"
#include "Model\UndoCommands\TrackCommands.h"
#include "QUndoStack"
#include "QUndoView"


class ControllerCommands : public IController {
	Q_OBJECT
	public:
		ControllerCommands(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
		~ControllerCommands();

	public slots:
		void receiveAddTrackCommand(QPoint pos);
		void receiveRemoveTrackCommand(IModelTrackedTrajectory* traj);
		void receiveRemoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber);
		void receiveMoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber, QPoint oldPos);
		void receiveSwapIdCommand(IModelTrackedTrajectory* traj0, IModelTrackedTrajectory* traj1);
		void receiveFixTrackCommand(IModelTrackedTrajectory* traj, bool toggle);

		void receiveUndo();
		void receiveRedo();

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