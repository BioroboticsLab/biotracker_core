#include "TrackedElementView.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "QBrush"
#include "QPainter"

TrackedElementView::TrackedElementView(QGraphicsItem *parent, IController *controller, IModel *model) :
    IViewTrackedComponent(parent, controller, model)
{
    TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());
    m_boundingRect = QRectF(0,0, 100, 100);

    setAcceptHoverEvents(true);
}

QRectF TrackedElementView::boundingRect() const
{

    return m_boundingRect;
}

void TrackedElementView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	TrackedTrajectory *all = dynamic_cast<TrackedTrajectory *>(getModel());
	if (!all)
		return;

	for (int i = 0; i < all->numberOfChildrean(); i++) {
		TrackedTrajectory *t = (TrackedTrajectory *)all->getChild(i);
		TrackedElement *elem = (TrackedElement *)t->getLastChild();
		int x = elem->getFishPose().position_cm().x;
		int y = elem->getFishPose().position_cm().y;
		QRectF rec = QRectF(x, y, 20, 20);

		QBrush brush(Qt::blue);

		if (elem->getPressedStatus())
		{
			brush.setColor(Qt::red);
		}
		else
		{
			brush.setColor(Qt::green);
		}

		painter->fillRect(rec, brush);
		painter->drawRect(rec);
	}
}

void TrackedElementView::getNotified()
{
    update();
}

void TrackedElementView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	/*
    TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());

    elem->pressed();
    update();

    QGraphicsObject::hoverEnterEvent(event);*/
}

void TrackedElementView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	/*
    TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());

    elem->notPressed();
    update();

    QGraphicsObject::hoverLeaveEvent(event);*/
}

