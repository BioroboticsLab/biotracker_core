#ifndef TRACKCOMMANDS_H
#define TRACKCOMMANDS_H

#pragma once

#include "QUndoCommand"
#include "Interfaces\IModel\IModelTrackedTrajectory.h"
#include "QPoint"

class AddTrackCommand : public QUndoCommand
{
public:
	AddTrackCommand(int id, QPoint pos,
		QUndoCommand *parent = 0);
	void undo() override;
	void redo() override;

private:
	IModelTrackedTrajectory* _traj;
	int _id;
};

class RemoveTrackCommand : public QUndoCommand
{
public:
	RemoveTrackCommand(IModelTrackedTrajectory* traj,
		QUndoCommand *parent = 0);

	void undo() override;
	void redo() override;

private:
	IModelTrackedTrajectory* _traj;
};

class RemoveElementCommand : public QUndoCommand
{
public:
	RemoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber,
		QUndoCommand *parent = 0);

	void undo() override;
	void redo() override;

private:
	IModelTrackedTrajectory* _traj;
	uint _frameNumber;
};

class MoveElementCommand : public QUndoCommand
{
public:
	MoveElementCommand(IModelTrackedTrajectory* traj, uint frameNumber, QPoint newPos,
		QUndoCommand *parent = 0);

	void undo() override;
	void redo() override;

private:
	IModelTrackedTrajectory* _traj;
	uint _frameNumber;
	QPoint _oldPos;
	QPoint _newPos;
};

class SwapTrackIdCommand : public QUndoCommand
{
public:
	SwapTrackIdCommand(IModelTrackedTrajectory* traj0, IModelTrackedTrajectory* traj1,
		QUndoCommand *parent = 0);

	void undo() override;
	void redo() override;

private:
	IModelTrackedTrajectory* _traj0;
	IModelTrackedTrajectory* _traj1;
};

class FixTrackCommand : public QUndoCommand
{
public:
	FixTrackCommand(IModelTrackedTrajectory* traj, bool isFixed,
		QUndoCommand *parent = 0);

	void undo() override;
	void redo() override;

private:
	IModelTrackedTrajectory* _traj;
	bool _isFixed;

};

#endif //TRACKCOMMANDS_H
