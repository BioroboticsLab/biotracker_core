#ifndef TRACKCOMMANDS_H
#define TRACKCOMMANDS_H

#pragma once

#include "QUndoCommand"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "QPoint"

// http://doc.qt.io/qt-5/qundo.html

/**
 *	Add track command with data to do and undo:
 *	position, id, traj, added
 */
class AddTrackCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    AddTrackCommand(int id, QPoint pos, QUndoCommand* parent = 0);
    void undo() override;
    void redo() override;
signals:
    void emitAddTrajectory(QPoint pos);
    void emitValidateTrajectory(int id);
    void emitRemoveTrajectoryId(int id);

private:
    IModelTrackedTrajectory* _traj;
    int                      _id;
    QPoint                   _pos;
    bool                     _added;
};

/**
 *	Remove Track command with data to do and undo:
 *	traj
 */
class RemoveTrackCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    RemoveTrackCommand(IModelTrackedTrajectory* traj,
                       QUndoCommand*            parent = 0);

    void undo() override;
    void redo() override;
signals:
    void emitValidateTrajectory(int id);
    void emitRemoveTrajectory(IModelTrackedTrajectory* traj);

private:
    IModelTrackedTrajectory* _traj;
};

/**
 *	Remove Entity command with data to do and undo:
 *	traj, framenumber
 */
class RemoveElementCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    RemoveElementCommand(IModelTrackedTrajectory* traj,
                         uint                     frameNumber,
                         QUndoCommand*            parent = 0);

    void undo() override;
    void redo() override;
signals:
    void emitValidateEntity(IModelTrackedTrajectory* traj, uint frameNumber);
    void emitRemoveElement(IModelTrackedTrajectory* traj, uint frameNumber);

private:
    IModelTrackedTrajectory* _traj;
    uint                     _frameNumber;
};

/**
 *	Move Entity command with data to do and undo:
 *	traj, oldPos, newPos, hwo many left to move (for a multimove), framenuber
 */
class MoveElementCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    MoveElementCommand(IModelTrackedTrajectory* traj,
                       uint                     frameNumber,
                       QPoint                   oldPos,
                       QPoint                   newPos,
                       int                      toMove,
                       QUndoCommand*            parent = 0);

    void undo() override;
    void redo() override;
    // bool mergeWith(const QUndoCommand *command) override;
signals:
    void emitMoveElement(IModelTrackedTrajectory* traj,
                         uint                     _frameNumber,
                         QPoint                   pos,
                         int                      toMove);

private:
    IModelTrackedTrajectory* _traj;
    uint                     _frameNumber;
    QPoint                   _oldPos;
    QPoint                   _newPos;
    int                      _toMove;
};

/**
 *	Swap Track Id command with data to do and undo:
 *	traj 0 and traj 1
 */
class SwapTrackIdCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    SwapTrackIdCommand(IModelTrackedTrajectory* traj0,
                       IModelTrackedTrajectory* traj1,
                       QUndoCommand*            parent = 0);

    void undo() override;
    void redo() override;

signals:
    void emitSwapIds(IModelTrackedTrajectory* traj0,
                     IModelTrackedTrajectory* traj1);

private:
    IModelTrackedTrajectory* _traj0;
    IModelTrackedTrajectory* _traj1;
};

/**
 *	Fix Track command with data to do and undo:
 *	fix toggle and traj
 */
class FixTrackCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    FixTrackCommand(IModelTrackedTrajectory* traj,
                    bool                     isFixed,
                    QUndoCommand*            parent = 0);

    void undo() override;
    void redo() override;
signals:
    void emitFixTrack(IModelTrackedTrajectory* traj, bool toggle);

private:
    IModelTrackedTrajectory* _traj;
    bool                     _isFixed;
};

/**
 *	Rotate Entity command with data to do and undo:
 *	traj, old angle, new angle, fraemenumber
 */
class RotateEntityCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    RotateEntityCommand(IModelTrackedTrajectory* traj,
                        double                   oldAngleDeg,
                        double                   newAngleDeg,
                        uint                     frameNumber,
                        QUndoCommand*            parent = 0);

    void undo() override;
    void redo() override;
signals:
    void emitEntityRotation(IModelTrackedTrajectory* traj,
                            double                   angle,
                            uint                     frameNumber);

private:
    IModelTrackedTrajectory* _traj;
    double                   _oldAngle;
    double                   _newAngle;
    uint                     _frameNumber;
};

#endif // TRACKCOMMANDS_H
