#include "TrackedElementView.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackingRectElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "Model/TrackingAlgorithm/property/Rectification.h"
#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>

class QGraphicsSceneHoverEvent;

TrackedElementView::TrackedElementView(QGraphicsItem *parent, IController *controller, IModel *model) :
    IViewTrackedComponent(parent, controller, model) 
{
    TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());
    m_boundingRect = QRectF(0,0, 100, 100);

	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::MouseButtons::enum_type::LeftButton);
	_watchingDrag = 0;
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
	TrackedTrajectory *all = dynamic_cast<TrackedTrajectory *>(getModel());
	if (!all)
		return;

	for (int i = 0; i < all->numberOfChildrean(); i++) {
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


			painter->setPen(QPen(Qt::yellow, 5));
			QLineF angleline;
			angleline.setP1(QPointF(x, y));
			angleline.setAngle(elem->getFishPose().orientation_deg());
			angleline.setLength(80);
			painter->drawLine(angleline);
		}

		TrackingRectElement *r = dynamic_cast<TrackingRectElement *>(all->getChild(i));
		if (r) {
			QBrush brush(Qt::blue);
			painter->setPen(QPen(Qt::blue, 3));
			if (r->getPressedStatus()) {
				painter->setPen(QPen(Qt::red, 3));
			}

			auto pair = _rectification.find(r->getId());
			if (pair == _rectification.end()) {
				std::shared_ptr<QGraphicsRectItem> ri = std::make_shared<QGraphicsRectItem>(QRect(r->getX() - 10, r->getY() - 10, 20, 20), this);
				ri->setAcceptHoverEvents(true);
				ri->setBrush(brush);
				ri->installSceneEventFilter(this);
				
				_rectification.insert(std::pair<int, std::shared_ptr<QGraphicsRectItem>>(r->getId(), ri));
			}

			//painter->drawRect(*rect);
			painter->setPen(QPen(Qt::blue, 3));
		}
	}
}

void TrackedElementView::getNotified()
{
    update();
}

bool isInRect(TrackingRectElement *r, const QPointF& p) {
	if (std::abs(r->getX() - p.x()) <= 10 && std::abs(r->getY() - p.y()) <= 10)
		return true;
	return false;
}

bool notIsInRect(TrackingRectElement *r, const QPointF& p) {
	if (std::abs(r->getX() - p.x()) > 7 && std::abs(r->getY() - p.y()) > 7)
		return true;
	return false;
}

bool TrackedElementView::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {

	//int et = event->type();
	//std::cout << et << std::endl;
	if (event->type() == QEvent::GraphicsSceneMousePress) {

		QGraphicsRectItem *r = dynamic_cast<QGraphicsRectItem *>(watched);
		if (r) {
			for (std::map<int, std::shared_ptr<QGraphicsRectItem>>::iterator it = _rectification.begin(); it != _rectification.end(); ++it) {
				if (it->second.get() == watched) {
					QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent*)event;
					_watchingDrag = watched;
					_dragX = e->pos().x();
					_dragY = e->pos().y();
					//std::cout << "Clicked corner " << it->first << " at " << _dragX << " , " << _dragY << '\n';
				}
			}
		}
	}
	if (event->type() == QEvent::GraphicsSceneMouseRelease) {

		QGraphicsRectItem *r = dynamic_cast<QGraphicsRectItem *>(watched);
		if (r) {
			//Find the one which was dragged
			for (std::map<int, std::shared_ptr<QGraphicsRectItem>>::iterator it = _rectification.begin(); it != _rectification.end(); ++it) {
				if (it->second.get() == _watchingDrag) {
					QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent*)event;
					int nowX = e->pos().x();
					int nowY = e->pos().y();
					it->second->setX(it->second->x() + nowX - _dragX);
					it->second->setY(it->second->y() + nowY - _dragY);

					//Find the one node in the datastructure which needs updating
					//This is a horribly wrong place to manage this. Not view stuff!
					TrackedTrajectory *all = dynamic_cast<TrackedTrajectory *>(getModel());
					if (!all)
						return true;
					for (int i = 0; i < all->numberOfChildrean(); i++) {
						TrackingRectElement *r = dynamic_cast<TrackingRectElement *>(all->getChild(i));
						if (r && r->getId() == it->first) {
							r->updateXY(nowX - _dragX, nowY - _dragY);
							Rectification::instance().setArea();
							Rectification::instance().setupRecitification(100, 100, 2040, 2040);
							std::cout << "Updating id " << r->getId() << std::endl;
						}
					}

					//std::cout << "Released corner " << it->first << " at " << nowX << " , " << nowY << '\n';
				}
			}
		}
	}
	return true;
}

bool TrackedElementView::eventFilter(QObject *target, QEvent *event)
{
	return true;// TrackedElementView::eventFilter(target, event);
}

void TrackedElementView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{	
	
	/*
	const QPointF event_scene_pos = event->scenePos();


	TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(getModel());
	if (t) {
		for (int i = 0; i < t->numberOfChildrean(); i++) {
			TrackingRectElement *r = dynamic_cast<TrackingRectElement *>(t->getChild(i));
			if (r) {
				if (isInRect(r, event_scene_pos)) {
					r->pressed();
					update();
					QGraphicsObject::hoverEnterEvent(event);
				}
			}
		}
	}*/
}

void TrackedElementView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	/*
	const QPointF event_scene_pos = event->scenePos();

	TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(getModel());
	if (t) {
		for (int i = 0; i < t->numberOfChildrean(); i++) {
			TrackingRectElement *r = dynamic_cast<TrackingRectElement *>(t->getChild(i));
			if (r) {
				if (notIsInRect(r, event_scene_pos)) {
					r->notPressed();
				}
			}
		}
	}
	update();
	QGraphicsObject::hoverEnterEvent(event);*/
}

