#ifndef TRACKEDCOMPONENTVIEW_H
#define TRACKEDCOMPONENTVIEW_H

#pragma once

#include "Interfaces/IView/IViewTrackedComponent.h"
#include "Interfaces/ENUMS.h"
#include "QPoint"
#include "QSignalMapper"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "View/ComponentShape.h"

/**
* This class inherits from the IViewTrackedComponent class and is therefore part of the Composite Pattern.
* This view is visualized in the media panel of the main window on top of the  loaded medium.
* It creates a component shape for each trajectory. The component shape is the visualized current entity of the trajectory.
* Each time the tracking plugins singals tracking done to the core app, all component shapes are updated (updateshapes(framnumber))
*/
class TrackedComponentView : public IViewTrackedComponent
{
	Q_OBJECT
public:
	TrackedComponentView(QGraphicsItem *parent = 0, IController *controller = 0, IModel *model = 0);

	// QGraphicsItem interface
public:
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	///creates initial componentshapes
	void createChildShapesAtStart();

	/// connect a newly created component shape to all slots it needs and set permissions
	void connectShape(ComponentShape* shape);

	// check if trajectory already has corresponding component shape
	bool checkTrajectory(IModelTrackedTrajectory* trajectory);

	// IView interface
	void setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);

	/**
	* SIGNALS
	*/
signals:
	void emitUpdateCornersChanged(int id, int relX, int relY);

	/// signal to commands component to add a new trajectory
	void emitAddTrajectory(QPoint pos, int id);

	/// signal to commands component to swap ID's of two trajectories
	void emitSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

	/**
	* SLOTS
	*/
	public slots:
	// IViewTrackedComponent interface
	void getNotified() override;

	/// updates dimensions on load of new medium
	void rcvDimensionUpdate(int x, int y);

	////////////// contextmenu actions (must be parameterless because Qt actions require it)

	/// add a new trajectory --> view updates componentshapes and generates a new component shape for that trajectory
	void addTrajectory();

	/// swap id's of excatcly two selected trajectories --> also componetshapes
	void swapIds();

	/// remove all selected trajectories --> also removes componentshapes
	void removeTrajectories();

	/// unmark all component shapes (border from fat to slim)
	void unmarkAll();

	//////////////////////////////////////////////////////////////////////////////

	/// update shapes when receiving tracking done
	void updateShapes(uint framenumber);

	/// broadcast the movment to all selected componentshapes so they are moved too
	void receiveBroadcastMove();

	/// receive toggle to hide or show the view and all its component shapes
	void receiveViewSwitch(bool lever);

	/// select all tracks
	void receiveSelectAll();

	/*
	*  set dimensions  of componentshapes
	*/

	/// set dimensions for all component shapes
	void receiveTrackDimensionsAll(int width, int height);

	/// set dimensions for selected component shapes
	void receiveTrackDimensionsSelected(int width, int height);

	/// set dimensions for to default for all componentshapes
	void receiveTrackDimensionsSetDefault();

	/*
	*  Change Color of componentshapes
	*/

	/// open color dialog and set border color for all componentshapes
	void receiveColorChangeBorderAll();

	/// open color dialog and set border color for selected componentshapes
	void receiveColorChangeBorderSelected();

	/// open color dialog and set fill color for all componentshapes
	void receiveColorChangeBrushAll();

	/// open color dialog and set fill color for all componentshapes
	void receiveColorChangeBrushSelected();

	/*
	*  Tracing of componentshapes
	*/

	/// set tracing history length for all componentshapes
	void receiveTracingHistoryLength(int history);

	/// set tracing style for all componentshapes
	void receiveTracingStyle(QString style);

	/// set tracing steps for all componentshapes
	void receiveTracingSteps(int steps);

	/// set tracing time degradation style for all componentshapes
	void receiveTracingTimeDegradation(QString degradation);

	/// toggle show framenumber of tracers for all componentshapes
	void receiveTracerFrameNumber(bool toggle);

	/// set tracing proportions for all componentshapes
	void receiveTracerProportions(float proportion);

	/// set tracer orientation line for all componentshapes
	void receiveTracerOrientationLine(bool toggle);

	/*
	*  Misc
	*/

	/// toggle antialiasing for all componentshapes
	void receiveToggleAntialiasingEntities(bool toggle);

	/// set ignore zoom for all componentshapes (unused)
	void receiveIgnoreZoom(bool toggle);

	/// toggle orientation line for all component shapes
	void receiveTrackOrientationLine(bool toggle);

	/// toggle show id for all component shapes
	void receiveTrackShowId(bool toggle);

public:
	/// updates tracking data model when new trakcing plugin is loaded
	void setNewModel(IModel *model) override { setModel(model); };

private:
	QRectF m_boundingRect;                                             /**< bounding rect of the view */

	std::map<int, std::shared_ptr<QGraphicsRectItem>> _rectification;  /**< id of the component */
	QGraphicsItem *_watchingDrag = nullptr;                            /**< unused drag bool */
	int _dragX = 0;                                                    /**< unused drag x-coordinate */
	int _dragY = 0;                                                    /**< unused drag y-coordinate */
	std::map<ENUMS::COREPERMISSIONS, bool> m_permissions;              /**< list of permissions to set for new component shapes */
	QPoint lastClickedPos;                                             /**< last clicked position in the view */

	// QGraphicsItem interface
protected:
	bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;
	bool eventFilter(QObject * target, QEvent * event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	int m_currentFrameNumber = 0;                                      /**< current visualized framenumber of the medium */
};

#endif // TRACKEDCOMPONENTVIEW_H
