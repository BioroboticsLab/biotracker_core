#include "Controller\ControllerCommands.h"

ControllerCommands::ControllerCommands(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
}

ControllerCommands::~ControllerCommands()
{
}

void ControllerCommands::receiveRemoveTrackCommand(IModelTrackedTrajectory * traj)
{
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
}

void ControllerCommands::receiveRedo()
{
}

void ControllerCommands::connectControllerToController()
{
}

void ControllerCommands::createModel()
{
}

void ControllerCommands::createView()
{
}
