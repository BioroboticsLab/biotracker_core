#include "TrackCommands.h"

#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "QUndoCommand"
#include "QPoint"
#include "QDebug"

AddTrackCommand::AddTrackCommand(int id, QPoint pos, QUndoCommand *parent)
	:QUndoCommand(parent), _id(id), _pos(pos)
{
	_added = false;
	setText(QString("Add trajectory ") + QString::number(id));
}

void AddTrackCommand::undo()
{
	emitRemoveTrajectoryId(_id);
}

void AddTrackCommand::redo()
{
	if (!_added) {
		Q_EMIT emitAddTrajectory(_pos);
		_added = true;
	}
	else {
		emitValidateTrajectory(_id);
	}
}

RemoveTrackCommand::RemoveTrackCommand(IModelTrackedTrajectory* traj, QUndoCommand *parent)
	:QUndoCommand(parent), _traj(traj)
{
	setText(QString("Remove trajectory ") + QString::number(traj->getId()));
}

void RemoveTrackCommand::undo()
{
	emitValidateTrajectory(_traj->getId());
}

void RemoveTrackCommand::redo()
{
	emitRemoveTrajectory(_traj);
}

RemoveElementCommand::RemoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber, QUndoCommand *parent)
	:QUndoCommand(parent), _traj(traj), _frameNumber(frameNumber)
{


	setText("Remove entity " + QString::number(_frameNumber) + " of trajectory " + QString::number(traj->getId()));
}

void RemoveElementCommand::undo()
{
	emitValidateEntity(_traj, _frameNumber);
}

void RemoveElementCommand::redo()
{
	emitRemoveElement(_traj, _frameNumber);
}

MoveElementCommand::MoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber, QPoint oldPos,  QPoint newPos, int toMove, QUndoCommand *parent)
	:QUndoCommand(parent), _traj(traj), _frameNumber(frameNumber), _oldPos(oldPos), _newPos(newPos), _toMove(toMove)
{
	setText("Move entity " + QString::number(_frameNumber) + " of trajectory " + QString::number(traj->getId()) + " to " + QString::number(_newPos.x()) + ", " + QString::number(_newPos.y()));
}

void MoveElementCommand::undo()
{
	emitMoveElement(_traj, _frameNumber, _oldPos, _toMove);
}

void MoveElementCommand::redo()
{
	emitMoveElement(_traj, _frameNumber, _newPos, _toMove);
	_toMove = 1;
}

/*bool MoveElementCommand::mergeWith(const QUndoCommand * command)
//{
//	const MoveElementCommand *moveCommandToMerge = static_cast<const MoveElementCommand *>(command);
//	IModelTrackedTrajectory* trajToMerge = moveCommandToMerge->_traj;
//	uint frameNumberToMerge = moveCommandToMerge->_frameNumber;
//
//	if ((_traj != trajToMerge) || (frameNumberToMerge != _frameNumber)) {
//		return false;
//	}
//
//	_oldPos
//
}*/

SwapTrackIdCommand::SwapTrackIdCommand(IModelTrackedTrajectory* traj0, IModelTrackedTrajectory* traj1, QUndoCommand *parent)
	:QUndoCommand(parent), _traj0(traj0), _traj1(traj1)
{
	setText("Swap id's of " + QString::number(_traj0->getId()) + " and " + QString::number(_traj1->getId()));
}

void SwapTrackIdCommand::undo()
{
	emitSwapIds(_traj1, _traj0);
}

void SwapTrackIdCommand::redo()
{
	emitSwapIds(_traj0, _traj1);
}

FixTrackCommand::FixTrackCommand(IModelTrackedTrajectory* traj, bool isFixed, QUndoCommand *parent)
	:QUndoCommand(parent), _traj(traj), _isFixed(isFixed)
{
	if (_isFixed) {
		setText("Fix tracking data of trajectory " + QString::number(_traj->getId()));
	}
	else {
		setText("Unfix tracking data of trajectory " + QString::number(_traj->getId()));

	}
}

void FixTrackCommand::undo()
{
	emitFixTrack(_traj, !_isFixed);
}

void FixTrackCommand::redo()
{
	emitFixTrack(_traj, _isFixed);
}

RotateEntityCommand::RotateEntityCommand(IModelTrackedTrajectory* traj, double oldAngle, double angle, uint frameNumber, QUndoCommand *parent)
	:QUndoCommand(parent), _traj(traj), _newAngle(angle), _oldAngle(oldAngle), _frameNumber(frameNumber)
{
	setText("Rotate entity "+ QString::number(_frameNumber) + " of trajectory " + QString::number(_traj->getId()) + " to " + QString::number(_newAngle));
}

void RotateEntityCommand::undo()
{
	emitEntityRotation(_traj, _oldAngle, _frameNumber);
}

void RotateEntityCommand::redo()
{
	emitEntityRotation(_traj, _newAngle, _frameNumber);
}