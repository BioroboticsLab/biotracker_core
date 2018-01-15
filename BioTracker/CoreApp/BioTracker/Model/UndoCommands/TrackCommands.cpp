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
	qDebug() << "undo add";
	emitRemoveTrajectoryId(_id);
}

void AddTrackCommand::redo()
{
	if (!_added) {
		qDebug() << "do add" << _id;
		Q_EMIT emitAddTrajectory(_pos);
		_added = true;
	}
	else {
		qDebug() << "redo add" << _id;
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
	qDebug() << "undo rmt";
	emitValidateTrajectory(_traj->getId());
}

void RemoveTrackCommand::redo()
{
	qDebug() << "do rmt";
	emitRemoveTrajectory(_traj);
}

RemoveElementCommand::RemoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber, QUndoCommand *parent)
	:QUndoCommand(parent), _traj(traj), _frameNumber(frameNumber)
{


	setText("Remove entity " + QString::number(_frameNumber) + " of trajectory " + QString::number(traj->getId()));
}

void RemoveElementCommand::undo()
{
	qDebug() << "undo rme";
	emitValidateEntity(_traj, _frameNumber);
}

void RemoveElementCommand::redo()
{
	qDebug() << "do rme";
	emitRemoveElement(_traj, _frameNumber);
}

MoveElementCommand::MoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber, QPoint oldPos,  QPoint newPos, int toMove, QUndoCommand *parent)
	:QUndoCommand(parent), _traj(traj), _frameNumber(frameNumber), _oldPos(oldPos), _newPos(newPos), _toMove(toMove)
{
	setText("Move entity " + QString::number(_frameNumber) + " of trajectory " + QString::number(traj->getId()) + " to " + QString::number(_newPos.x()) + ", " + QString::number(_newPos.y()));
}

void MoveElementCommand::undo()
{
	qDebug() << "undo mv";
	emitMoveElement(_traj, _frameNumber, _oldPos, _toMove);
}

void MoveElementCommand::redo()
{
	qDebug() << "do mv";
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
	qDebug() << "undo swap";
	emitSwapIds(_traj1, _traj0);
}

void SwapTrackIdCommand::redo()
{
	qDebug() << "do swap";
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
	qDebug() << "undo fix";
	emitFixTrack(_traj, !_isFixed);
}

void FixTrackCommand::redo()
{
	qDebug() << "do fix ";
	emitFixTrack(_traj, _isFixed);
}

RotateEntityCommand::RotateEntityCommand(IModelTrackedTrajectory* traj, double oldAngle, double angle, uint frameNumber, QUndoCommand *parent)
	:QUndoCommand(parent), _traj(traj), _newAngle(angle), _oldAngle(oldAngle), _frameNumber(frameNumber)
{
	setText("Rotate entity "+ QString::number(_frameNumber) + " of trajectory " + QString::number(_traj->getId()) + " to " + QString::number(_newAngle));
}

void RotateEntityCommand::undo()
{
	qDebug() << "undo rot";
	emitEntityRotation(_traj, _oldAngle, _frameNumber);
}

void RotateEntityCommand::redo()
{
	qDebug() << "do rot ";
	emitEntityRotation(_traj, _newAngle, _frameNumber);
}