#include "TrackCommands.h"

#include "Interfaces\IModel\IModelTrackedTrajectory.h"
#include "QUndoCommand"
#include "QPoint"
#include "QDebug"

AddTrackCommand::AddTrackCommand(int id, QPoint pos,  QUndoCommand *parent)
	:QUndoCommand(parent)
{
	
	
	setText(QObject::tr("Add track"));
}

void AddTrackCommand::undo()
{
	qDebug() << "undo add";
}

void AddTrackCommand::redo()
{
	qDebug() << "do add";
}

RemoveTrackCommand::RemoveTrackCommand(IModelTrackedTrajectory* traj, QUndoCommand *parent)
	:QUndoCommand(parent)
{
	//IModelTrackedTrajectory* traj = new IModelTrackedTrajectory();


	setText(QObject::tr("Remove track"));
}

void RemoveTrackCommand::undo()
{
	qDebug() << "undo rmt";
}

void RemoveTrackCommand::redo()
{
	qDebug() << "do rmt";
}

RemoveElementCommand::RemoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber, QUndoCommand *parent)
	:QUndoCommand(parent)
{
	//IModelTrackedTrajectory* traj = new IModelTrackedTrajectory();


	setText(QObject::tr("Remove element"));
}

void RemoveElementCommand::undo()
{
	qDebug() << "undo rme";
}

void RemoveElementCommand::redo()
{
	qDebug() << "do rme";
}

MoveElementCommand::MoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber, QPoint newPos, QUndoCommand *parent)
	:QUndoCommand(parent)
{
	//IModelTrackedTrajectory* traj = new IModelTrackedTrajectory();


	setText(QObject::tr("Move"));
}

void MoveElementCommand::undo()
{
	qDebug() << "undo mv";
}

void MoveElementCommand::redo()
{
	qDebug() << "do mv";
}

SwapTrackIdCommand::SwapTrackIdCommand(IModelTrackedTrajectory* traj0, IModelTrackedTrajectory* traj1, QUndoCommand *parent)
	:QUndoCommand(parent)
{
	//IModelTrackedTrajectory* traj = new IModelTrackedTrajectory();


	setText(QObject::tr("Swap"));
}

void SwapTrackIdCommand::undo()
{
	qDebug() << "undo mv";
}

void SwapTrackIdCommand::redo()
{
	qDebug() << "do mv";
}

FixTrackCommand::FixTrackCommand(IModelTrackedTrajectory* traj, bool isFixed, QUndoCommand *parent)
	:QUndoCommand(parent)
{
	//IModelTrackedTrajectory* traj = new IModelTrackedTrajectory();


	setText(QObject::tr("Fix"));
}

void FixTrackCommand::undo()
{
	qDebug() << "undo fix";
}

void FixTrackCommand::redo()
{
	qDebug() << "do fix ";
}