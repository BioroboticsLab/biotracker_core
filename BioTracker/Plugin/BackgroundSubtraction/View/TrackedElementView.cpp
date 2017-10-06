#include "TrackedElementView.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackingRectElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "QBrush"
#include "QPainter"
#include "QGraphicsScene"
#include <QGraphicsSceneHoverEvent>
#include "settings/Settings.h"
#include "Model/TrackingAlgorithm/property/ParamNames.h"
#include "util/misc.h"
#include "qDebug.h"

class QGraphicsSceneHoverEvent;

TrackedElementView::TrackedElementView(QGraphicsItem *parent, IController *controller, IModel *model) :
    IViewTrackedComponent(parent, controller, model) 
{
    TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());
    m_boundingRect = QRectF(0,0, 100, 100);
} 

void TrackedElementView::rcvDimensionUpdate(int x, int y) {
	m_boundingRect = QRectF(0, 0, x, y);
	update();
}

QRectF TrackedElementView::boundingRect() const
{

    return m_boundingRect;
}


void TrackedElementView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsScene *scene = this->scene();

	//scene->setSceneRect(boundingRect());

	TrackedTrajectory *all = dynamic_cast<TrackedTrajectory *>(getModel());
	if (!all)
		return;

	for (int i = 0; i < all->size(); i++) {
		TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(all->getChild(i));
		if (t) {
			TrackedElement *elem = (TrackedElement *)t->getLastChild();
			int x = elem->getFishPose().position_px().x;
			int y = elem->getFishPose().position_px().y;
			int w = 40;
			int h = 40;
			QRectF rec = QRectF(x, y, 20, 20);

			QBrush brush(Qt::green);
			
			painter->setBrush(brush);
			painter->drawEllipse(x - w / 2, y - h / 2, w, h);

			//scene->addEllipse(x - w / 2, y - h / 2, w, h);


			painter->setPen(QPen(Qt::yellow, 5));
			QLineF angleline;
			angleline.setP1(QPointF(x, y));
			angleline.setAngle(elem->getFishPose().orientation_deg());
			angleline.setLength(80);
			painter->drawLine(angleline);
		}
	}

	QBrush brush(Qt::blue);
	painter->setPen(QPen(Qt::blue, 3));
}

void TrackedElementView::getNotified()
{
    update();
}

