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
		int w = 40;
		int h = 40;
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

		painter->setBrush(brush);
		painter->drawEllipse(x-w/2, y-h/2, w, h);


		painter->setPen(QPen(Qt::yellow, 5));
		QLineF angleline;
		angleline.setP1(QPointF(x, y));
		angleline.setAngle(elem->getFishPose().orientation_deg());
		angleline.setLength(80);
		painter->drawLine(angleline);
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

