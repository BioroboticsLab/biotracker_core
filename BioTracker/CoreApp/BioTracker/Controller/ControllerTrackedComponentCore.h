#ifndef CONTROLLERTRACKEDCOMPONENTCORE_H
#define CONTROLLERTRACKEDCOMPONENTCORE_H

#include "Interfaces/IController/IController.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"

class ControllerTrackedComponentCore : public IController
{
	Q_OBJECT
	public:
		ControllerTrackedComponentCore(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);

		IView *getTrackingElementsWidgetCore();

		void addModel(IModel* model);

		IModel* getCoreParameter();

	signals:
		// signal to ctrPlugin to remove trajectory
		void emitRemoveTrajectory(IModelTrackedTrajectory* trajectory);

		// signal to ctrPlugin to remove track entity
		void emitRemoveTrackEntity(IModelTrackedTrajectory* trajectory, uint frameNumber);

		// signal to ctrPlugin to add trajectory
		void emitAddTrajectory(QPoint pos, int id);

		void emitMoveElement(IModelTrackedTrajectory* trajectory, QPoint oldPos, QPoint newPos, uint frameNumber, int toMove);

		void emitSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

		void emitToggleFixTrack(IModelTrackedTrajectory* trajectory, bool toggle);

		void emitTrackNumber(int number);

		void emitDimensionUpdate(int x, int y);


	public Q_SLOTS:

        //A (different) plugin has been loaded. Here, the controller needs to inform the view
        void receiveOnPluginLoaded();

		//this slot gets triggered when the core-view needs to update with the current frame
		void receiveVisualizeTrackingModel(uint framenumber);


		//gets triggered when plugin sends permissions
		void setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);

		//gets triggered when trajectory is getting removed
		void receiveRemoveTrajectory(IModelTrackedTrajectory* trajectory);

		//gets triggered when a track entity is getting removed
		void receiveRemoveTrackEntity(IModelTrackedTrajectory* trajectory, uint frameNumber);

		void receiveAddTrajectory(QPoint pos, int id);

		void receiveMoveElement(IModelTrackedTrajectory* trajectory, QPoint oldPos, QPoint newPos, uint frameNumber, int toMove);

		void receiveSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

		void receiveToggleFixTrack(IModelTrackedTrajectory* trajectory, bool toggle);

		void receiveUpdateView();
		

		// IController interface
	protected:
		void createModel() override;
		void createView() override;
		void connectModelToController() override;
		void connectControllerToController() override;

		IView* m_parameterView;
		IModel* m_coreParameterModel;
};

#endif // CONTROLLERTRACKEDCOMPONENTCORE_H
