/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef CONTROLLERPLUGIN_H
#define CONTROLLERPLUGIN_H

#include "Interfaces/IController/IController.h"
#include "Interfaces/IBioTrackerPlugin.h"
#include "QThread"
#include "QQueue"
#include "QPoint"

enum EDIT { REMOVE_TRACK, REMOVE_ENTITY, ADD, MOVE, SWAP };

struct queueElement {
	EDIT type;
	QPoint pos;
	IModelTrackedTrajectory* trajectory0;
	IModelTrackedTrajectory* trajectory1;
	IModelTrackedComponent* element;
};

/**
 * This is the controller class of the Plugin Loader Component. This component is responsible for loading and managing BioTracker Plugins
 * The ControllerPlugin class is responsible for loading and connecting to BioTracker Plugins.
 */
class ControllerPlugin : public IController {
	Q_OBJECT
  public:
	ControllerPlugin(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
	~ControllerPlugin();

	void addToPluginList(QString str);

    /**
     * (This function
     * This function delivers a file path of a Plugin to the IModel class PluginLoader. If the Plugin could be loaded,
     * the functions additionally requests the name of the Plugin and sets it as an entry into an IView widget in the MainWindow.
     */
    void loadPluginFromFileName(QString str);

    /**
     * This function hands the received cv::Mat pointer and the current frame number to the PluginLoader.
     */
    void sendCurrentFrameToPlugin(std::shared_ptr<cv::Mat> mat, uint number);

	void selectPlugin(QString str);

signals:
	void emitRemoveTrajectory(IModelTrackedTrajectory* trajectory);
	void emitRemoveTrackEntity(IModelTrackedTrajectory* trajectory);
	void emitAddTrajectory(QPoint pos);
	void emitMoveElement(IModelTrackedTrajectory* element, QPoint pos);
	void emitSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

	void emitUpdateView();
	void signalCurrentFrameNumberToPlugin(uint frameNumber);

	// IController interface
  protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;

	void createPlugin();

	void connectPlugin();
	void disconnectPlugin();

  private Q_SLOTS:
	/**
	 *
	 * If Tracking is active and the tracking process was finished, the Plugin is able to emit a Signal that triggers this SLOT.
	 */
	void receiveTrackingDone();
	/**
	*
	* Receive command to remove a trajectory and put it in edit queue
	*/
	void  receiveRemoveTrajectory(IModelTrackedTrajectory* trajectory);
	/**
	*
	* Receive command to remove a track entity and put it in edit queue
	*/
	void  receiveRemoveTrackEntity(IModelTrackedTrajectory* trajectory);
	/**
	*
	* Receive command to add a trajectory and put it in edit queue
	*/
	void  receiveAddTrajectory(QPoint pos);
	/**
	*
	* Receive command to move a element in a trajectory and put it in edit queue
	*/
	void  receiveMoveElement(IModelTrackedTrajectory* trajectory, QPoint pos, int toMove);
	/**
	*
	* Receive command to swap two ID's and put it in edit queue
	*/
	void  receiveSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

	void  receivePauseState(bool state);

	void  receiveCurrentFrameNumberToPlugin(uint frameNumber);



  private:
	void loadPluginsFromPluginSubfolder();

    IBioTrackerPlugin* m_BioTrackerPlugin;

	QQueue<queueElement> m_editQueue;

	QPointer< QThread >  m_TrackingThread;

	bool m_paused = true;



};

#endif // CONTROLLERPLUGIN_H
