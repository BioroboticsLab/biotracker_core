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
* This class inherits from the IViewTrackedComponent class and is therefor part of the Composite Pattern.
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
	
	void createChildShapesAtStart();
	void connectShape(ComponentShape* shape);
	
	// IView interface
	void setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);

signals:
	void emitUpdateCornersChanged(int id, int relX, int relY);
	void emitAddTrajectory(QPoint pos, int id);
	void emitSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

	// IViewTrackedComponent interface
public slots:
	void getNotified() override;
	void rcvDimensionUpdate(int x, int y);
	// contextmenu actions
	void addTrajectory();
	void swapIds();
	void removeTrajectories();
	void unmarkAll();
	// update shapes when receiving tracking done
	void updateShapes(uint framenumber);
	//Move Tracks
	void receiveBroadcastMove();
	void receiveViewSwitch(bool lever);
	//Select all tracks
	void receiveSelectAll();
	//Track dimensions
	void receiveTrackDimensionsAll(int width, int height);
	void receiveTrackDimensionsSelected(int width, int height);
	void receiveTrackOrientationLine(bool toggle);
	void receiveTrackShowId(bool toggle);
	void receiveTrackDimensionsSetDefault();
	//Track ignore zoom
	void receiveIgnoreZoom(bool toggle);
	//Track color
	void receiveColorChangeBorderAll();
	void receiveColorChangeBorderSelected();
	void receiveColorChangeBrushAll();
	void receiveColorChangeBrushSelected();
	//Tracing
	void receiveTracingHistoryLength(int history);
	void receiveTracingStyle(QString style);
	void receiveTracingSteps(int steps);
	void receiveTracingTimeDegradation(QString degradation);
	void receiveTracerFrameNumber(bool toggle);
	//Tracing dimensions
	void receiveTracerProportions(float proportion);
	void receiveTracerOrientationLine(bool toggle);

	//Visual
	void receiveToggleAntialiasingEntities(bool toggle);


public:
	void setNewModel(IModel *model) override { setModel(model); };

private:
	QRectF m_boundingRect;

	std::map<int, std::shared_ptr<QGraphicsRectItem>> _rectification;
	QGraphicsItem *_watchingDrag = nullptr;
	int _dragX = 0;
	int _dragY = 0;
	std::map<ENUMS::COREPERMISSIONS, bool> m_permissions;
	QPoint lastClickedPos;

	// QGraphicsItem interface
protected:
	bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;
	bool eventFilter(QObject * target, QEvent * event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
	//void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;

	/*void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
	void dropEvent(QGraphicsSceneDragDropEvent *event) override;*/
	
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	//member
	bool permissionAdd;
	bool permissionSwap;

	int m_currentFrameNumber = 0;
	//QGraphicsTextItem* _cursorPosText;
};

#endif // TRACKEDCOMPONENTVIEW_H
