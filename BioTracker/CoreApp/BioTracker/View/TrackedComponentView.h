#ifndef TRACKEDCOMPONENTVIEW_H
#define TRACKEDCOMPONENTVIEW_H

#pragma once

#include "Interfaces/IView/IViewTrackedComponent.h"
#include "Interfaces/ENUMS.h"
#include "QPoint"
#include "QSignalMapper"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"

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
	//void connectShape(ComponentShape* shape);
	
	// IView interface
	void setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);

signals:
	void emitUpdateCornersChanged(int id, int relX, int relY);
	void emitAddTrajectory(QPoint pos);
	void emitSwapIds(IModelTrackedTrajectory* trajectory0, IModelTrackedTrajectory* trajectory1);

	// IViewTrackedComponent interface
public slots:
	void getNotified() override;
	void rcvDimensionUpdate(int x, int y);
	// contextmenu actions
	void addTrajectory();
	void swapIds();
	void removeTrajectories();
	// update shapes when receiving tracking done
	void updateShapes(uint framenumber);

	void receiveBroadcastMove();
	void receiveViewSwitch(bool lever);
	void receiveSelectAll();
	void receiveColorChangeBorderAll();
	void receiveColorChangeBrushAll();
	void receiveTracingHistoryLength(int history);
	void receiveTracingStyle(QString style);
	void receiveTracingSteps(int steps);

public:
	void setNewModel(IModel *model) override { setModel(model); };

private:
	QRectF m_boundingRect;

	std::map<int, std::shared_ptr<QGraphicsRectItem>> _rectification;
	QGraphicsItem *_watchingDrag;
	int _dragX;
	int _dragY;
	std::map<ENUMS::COREPERMISSIONS, bool> m_permissions;
	QPoint lastClickedPos;

	// QGraphicsItem interface
protected:
	bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;
	bool eventFilter(QObject * target, QEvent * event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

	/*void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
	void dropEvent(QGraphicsSceneDragDropEvent *event) override;*/
	
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	//member
	bool permissionAdd;
	bool permissionSwap;
};

#endif // TRACKEDCOMPONENTVIEW_H
