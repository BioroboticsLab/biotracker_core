#ifndef TRACKEDCOMPONENTVIEW_H
#define TRACKEDCOMPONENTVIEW_H

#include "Interfaces/IView/IViewTrackedComponent.h"

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

	// IViewTrackedComponent interface
	public Q_SLOTS:
	void getNotified() override;
	void rcvDimensionUpdate(int x, int y);

	public Q_SIGNAL:
	void emitUpdateCornersChanged(int id, int relX, int relY);
	void createChildShapesAtStart();
	void updateShapes(uint framenumber);

private:
	QRectF m_boundingRect;

	std::map<int, std::shared_ptr<QGraphicsRectItem>> _rectification;
	QGraphicsItem *_watchingDrag;
	int _dragX;
	int _dragY;

	// QGraphicsItem interface
protected:
	bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;
	bool eventFilter(QObject * target, QEvent * event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
	
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // TRACKEDCOMPONENTVIEW_H
