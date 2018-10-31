#ifndef CONTROLLERTRACKEDCOMPONENTCORE_H
#define CONTROLLERTRACKEDCOMPONENTCORE_H

#include "IControllerCfg.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"


/**
* The ControllerTrackedComponentCore class controls the component for visualizing TrackedComponents.
* It is basically a forwarder for permissions, user interaction signals (including tracking data manipulation).
*/

class ControllerTrackedComponentCore : public IControllerCfg
{
	Q_OBJECT
public:
	ControllerTrackedComponentCore(QObject *parent = 0, IBioTrackerContext *context = 0,
		ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);

	/**
	* Hook for the view to put in in the mainwindow's media panel each time a plugin is loaded
	*/
	IView *getTrackingElementsWidgetCore();

	/**
	* The model is actually the tracking data from the tracking plugin. This component only reads this data.
	* Is called when a new tracking plugin is loaded.
	*/
	void addModel(IModel* model);

	/**
	* Hook for the view to get the core parameters to set up new componentshapes with default settings
	*/
	IModel* getCoreParameter();

	/*
	* SIGNALS
	*/
signals:
	/// signal to ctrcommands to remove trajectory
	void emitRemoveTrajectory(IModelTrackedTrajectory* trajectory);

	/// signal to ctrcommands to remove entity
	void emitRemoveTrackEntity(IModelTrackedTrajectory* trajectory, uint frameNumber);

	/// signal to ctrcommands to add trajectory
	void emitAddTrajectory(QPoint pos, int id);

	/// signal to ctrcommands to move entity
	void emitMoveElement(IModelTrackedTrajectory* trajectory, QPoint oldPos, QPoint newPos, uint frameNumber, int toMove);

	/// signal to ctrcommands to swap track id's
	void emitSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

	/// signal to ctrcommands fix or unfix a traj
	void emitToggleFixTrack(IModelTrackedTrajectory* trajectory, bool toggle);

	/// signal to ctrcommands rotate entity
	void emitEntityRotation(IModelTrackedTrajectory*, double oldAngleDeg, double newAngleDeg, uint frameNumber);

	/// signal to ctrCoreParameter to show number of valid trrajectories

	void emitTrackNumber(int number);

	/// signal to view to update its dimensions
	void emitDimensionUpdate(int x, int y);

	//forward left-toolbar actions to view
	void emitAddTrack();
	void emitDeleteSelectedTracks();
	void emitSwapIdsToView();
	void emitSelectAll();
	void emitChangeColorBorder();
	void emitChangeColorFill();
	void emitGoToFrame(int frame);


	/*
	* SLOTS
	*/
	public Q_SLOTS:

	/// A (different) plugin has been loaded. Here, the controller needs to inform the view
	void receiveOnPluginLoaded();

	/// this slot gets triggered when the core-view needs to update with the current frame
	void receiveVisualizeTrackingModel(uint framenumber);

	/// gets triggered when plugin sends permissions and forwards it to the view
	void setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);

	/// gets triggered when a the view needs an update
	void receiveUpdateView();


	// IController interface
protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;

	//IView* m_parameterView;
	//IModel* m_coreParameterModel;
};

#endif // CONTROLLERTRACKEDCOMPONENTCORE_H
