#include "Controller\ControllerCommands.h"

ControllerCommands::ControllerCommands(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
}

ControllerCommands::~ControllerCommands()
{
}

void ControllerCommands::receiveAddTrackCommand(QPoint pos, int id) {
	AddTrackCommand* addCmd = new AddTrackCommand(id, pos);
	QObject::connect(addCmd, &AddTrackCommand::emitAddTrajectory, this, &ControllerCommands::emitAddTrajectory);
	QObject::connect(addCmd, &AddTrackCommand::emitValidateTrajectory, this, &ControllerCommands::emitValidateTrajectory);
	QObject::connect(addCmd, &AddTrackCommand::emitRemoveTrajectoryId, this, &ControllerCommands::emitRemoveTrajectoryId);

	_undoStack->push(addCmd);
}

void ControllerCommands::receiveRemoveTrackCommand(IModelTrackedTrajectory * traj)
{
	RemoveTrackCommand* rmtCmd = new RemoveTrackCommand(traj);
	/*QObject::connect(rmtCmd, &AddTrackCommand::emitAddTrajectory, this, &ControllerCommands::emitAddTrajectory);
	QObject::connect(rmtCmd, &AddTrackCommand::emitValidateTrajectory, this, &ControllerCommands::emitValidateTrajectory);
	QObject::connect(rmtCmd, &AddTrackCommand::emitRemoveTrajectoryId, this, &ControllerCommands::emitRemoveTrajectoryId);*/

	_undoStack->push(rmtCmd);
}

void ControllerCommands::receiveRemoveElementCommand(IModelTrackedTrajectory * traj, uint frameNumber)
{
}

void ControllerCommands::receiveMoveElementCommand(IModelTrackedTrajectory * traj, uint frameNumber, QPoint oldPos)
{
}

void ControllerCommands::receiveSwapIdCommand(IModelTrackedTrajectory * traj0, IModelTrackedTrajectory * traj1)
{
}

void ControllerCommands::receiveFixTrackCommand(IModelTrackedTrajectory * traj, bool toggle)
{
}

void ControllerCommands::receiveUndo()
{
	if (_undoStack->canUndo()) {
		_undoStack->undo();
	}
}

void ControllerCommands::receiveRedo()
{
}

void ControllerCommands::connectControllerToController()
{
}

void ControllerCommands::createModel()
{
	_undoStack = new QUndoStack(this);
}

void ControllerCommands::createView()
{
	_undoView = new QUndoView(_undoStack);
	_undoView->setWindowTitle("Command List");
	_undoView->setAttribute(Qt::WA_QuitOnClose, false);
	_undoView->show();
}

void ControllerCommands::connectModelToController()
{
}
