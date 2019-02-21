#include "Controller/ControllerCommands.h"
#include "QDebug"

ControllerCommands::ControllerCommands(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IControllerCfg(parent, context, ctr)
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
	QObject::connect(rmtCmd, &RemoveTrackCommand::emitValidateTrajectory, this, &ControllerCommands::emitValidateTrajectory);
	QObject::connect(rmtCmd, &RemoveTrackCommand::emitRemoveTrajectory, this, &ControllerCommands::emitRemoveTrajectory);

	_undoStack->push(rmtCmd);
}

void ControllerCommands::receiveRemoveTrackEntityCommand(IModelTrackedTrajectory * traj, uint frameNumber)
{
	RemoveElementCommand* rmeCmd = new RemoveElementCommand(traj, frameNumber);
	QObject::connect(rmeCmd, &RemoveElementCommand::emitValidateEntity, this, &ControllerCommands::emitValidateEntity);
	QObject::connect(rmeCmd, &RemoveElementCommand::emitRemoveElement, this, &ControllerCommands::emitRemoveTrackEntity);

	_undoStack->push(rmeCmd);
}

void ControllerCommands::receiveMoveElementCommand(IModelTrackedTrajectory* traj, QPoint oldPos, QPoint newPos, uint frameNumber, int toMove)
{
	MoveElementCommand* mvCmd = new MoveElementCommand(traj, frameNumber, oldPos, newPos, toMove);
	QObject::connect(mvCmd, &MoveElementCommand::emitMoveElement, this, &ControllerCommands::emitMoveElement);

	_undoStack->push(mvCmd);
}

void ControllerCommands::receiveSwapIdCommand(IModelTrackedTrajectory * traj0, IModelTrackedTrajectory * traj1)
{
	SwapTrackIdCommand* swapCmd = new SwapTrackIdCommand(traj0, traj1);
	QObject::connect(swapCmd, &SwapTrackIdCommand::emitSwapIds, this, &ControllerCommands::emitSwapIds);

	_undoStack->push(swapCmd);
}

void ControllerCommands::receiveFixTrackCommand(IModelTrackedTrajectory * traj, bool toggle)
{
	FixTrackCommand* fixCmd = new FixTrackCommand(traj, toggle);
	QObject::connect(fixCmd, &FixTrackCommand::emitFixTrack, this, &ControllerCommands::emitToggleFixTrack);

	_undoStack->push(fixCmd);
}

void ControllerCommands::receiveEntityRotation(IModelTrackedTrajectory * traj,double oldAngleDeg, double newAngleDeg, uint frameNumber)
{
	RotateEntityCommand* rotCmd = new RotateEntityCommand(traj, oldAngleDeg, newAngleDeg, frameNumber);
	QObject::connect(rotCmd, &RotateEntityCommand::emitEntityRotation, this, &ControllerCommands::emitEntityRotation);

	_undoStack->push(rotCmd);
}

void ControllerCommands::receiveUndo()
{
	if (_undoStack->canUndo()) {
		_undoStack->undo();
	}
	else {
		qDebug() << "CORE: Cannot undo the last command!";
	}
}

void ControllerCommands::receiveRedo()
{
	if (_undoStack->canRedo()) {
		_undoStack->redo();
	}
	else {
		qDebug() << "CORE: Cannot redo the next command!";
	}
}

void ControllerCommands::receiveClear()
{
	_undoStack->clear();
}

void ControllerCommands::receiveShowActionList()
{
	_undoView->show();
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
}

void ControllerCommands::connectModelToController()
{
}
