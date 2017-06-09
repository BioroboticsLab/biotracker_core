#include "TrackedElementView.h"
#include "Model/TrackedComponents/TrackedElement.h"
#include "Model/TrackedComponents/TrackedTrajectory.h"
#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>
#include <QMouseEvent>

class QGraphicsSceneHoverEvent;

TrackedElementView::TrackedElementView(QGraphicsItem *parent, IController *controller, IModel *model) :
    IViewTrackedComponent(parent, controller, model) 
{
    TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());
    m_boundingRect = QRectF(0,0, 100, 100);

	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::MouseButtons::enum_type::LeftButton);

	_showSelectorRec = false; 
	this->installEventFilter(this);
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

	//painter->drawRoundRect(&*ri);
	if (_showSelectorRec) {
		QColor color(Qt::cyan);
		color.setAlpha(150);
		QBrush brush(color);
		painter->fillRect(QRectF(_selectorRecStart, _selectorRecEnd), brush);
	}
}

void TrackedElementView::getNotified()
{
    update();
}

bool TrackedElementView::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
	if (event->type() == QEvent::GraphicsSceneMousePress) {
		QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent *)event;
		int x = e->pos().x();
		int y = e->pos().y();
		std::string note = "shift + left button press on: x="
			+ QString::number(x).toStdString() + " y=" + QString::number(y).toStdString();
		//initialize coordinates for selection tool
		_selectorRecStart.setX(e->pos().x());
		_selectorRecStart.setY(e->pos().y());
		_selectorRecEnd.setX(e->pos().x());
		_selectorRecEnd.setY(e->pos().y());
		_showSelectorRec = true;
		Q_EMIT update();
	}
	if (event->type() == QEvent::GraphicsSceneMouseMove) {
		QGraphicsSceneMouseEvent *e;
		if (_showSelectorRec && (e = dynamic_cast<QGraphicsSceneMouseEvent *>(event))) {
			_selectorRecEnd.setX(e->pos().x());
			_selectorRecEnd.setY(e->pos().y());
			Q_EMIT update();
		}
	}
	if (event->type() == QEvent::GraphicsSceneMouseRelease) {
		if (_showSelectorRec) {
			QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent*)event;
			_selectorRecEnd.setX(e->pos().x());
			_selectorRecEnd.setY(e->pos().y());
			_showSelectorRec = false;
			//next draw will delete rectangle!
			Q_EMIT update();
			std::string note = "selected area from " + QString::number(
				_selectorRecStart.x()).toStdString() + ":" +
				QString::number(_selectorRecStart.y()).toStdString()
				+ " to " + QString::number(_selectorRecEnd.x()).toStdString() + ":" +
				QString::number(_selectorRecEnd.y()).toStdString();
		}
	}
	return true;
}

bool TrackedElementView::eventFilter(QObject *target, QEvent *event)
{
	if (event->type() == QEvent::GraphicsSceneMousePress) {
		QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent *)event;
		int x = e->pos().x();
		int y = e->pos().y();
		std::string note = "shift + left button press on: x="
			+ QString::number(x).toStdString() + " y=" + QString::number(y).toStdString();
		//initialize coordinates for selection tool
		_selectorRecStart.setX(e->pos().x());
		_selectorRecStart.setY(e->pos().y());
		_selectorRecEnd.setX(e->pos().x());
		_selectorRecEnd.setY(e->pos().y());
		_showSelectorRec = true;
		Q_EMIT update();
	}
	if (event->type() == QEvent::GraphicsSceneMouseMove) {
		QGraphicsSceneMouseEvent *e;
		if (_showSelectorRec && (e = dynamic_cast<QGraphicsSceneMouseEvent *>(event))) {
			_selectorRecEnd.setX(e->pos().x());
			_selectorRecEnd.setY(e->pos().y());
			Q_EMIT update();
		}
	}
	if (event->type() == QEvent::GraphicsSceneMouseRelease) {
		if (_showSelectorRec) {
			QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent*)event;
			_selectorRecEnd.setX(e->pos().x());
			_selectorRecEnd.setY(e->pos().y());
			_showSelectorRec = false;
			//next draw will delete rectangle!
			Q_EMIT update();
			std::string note = "selected area from " + QString::number(
				_selectorRecStart.x()).toStdString() + ":" +
				QString::number(_selectorRecStart.y()).toStdString()
				+ " to " + QString::number(_selectorRecEnd.x()).toStdString() + ":" +
				QString::number(_selectorRecEnd.y()).toStdString();
		}
	}
	return true;// TrackedElementView::eventFilter(target, event);
}

void TrackedElementView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{	
}

void TrackedElementView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
}

