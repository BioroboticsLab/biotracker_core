#include "EllipseDescriptor.h"

#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>

#include "Model/AreaDescriptor/AreaInfoElement.h"
#include "util/misc.h"

EllipseDescriptor::EllipseDescriptor(IController *controller, IModel *model) :
	AreaDescriptor(controller, model)
{
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::MouseButtons::enum_type::LeftButton);

	_brush = QBrush(Qt::blue);


	_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
	for (int i = 0; i < 4; i++) {
		std::shared_ptr<QGraphicsRectItem> ri = std::make_shared<QGraphicsRectItem>(QRect(_v[i].x - 10, _v[i].y - 10, 20, 20), this);
		ri->setBrush(_brush);
	}

	updateEllipse();

	_isInit = false;
}

void EllipseDescriptor::init() {
	_rectificationMarkerOrig->setAcceptHoverEvents(true);
	_rectificationMarkerOrig->installSceneEventFilter(this);
	_rectificationMarkerEnd->setAcceptHoverEvents(true);
	_rectificationMarkerEnd->installSceneEventFilter(this);
	_rectificationMarkerOrig->setBrush(_brush);
	_rectificationMarkerEnd->setBrush(_brush);
}

void EllipseDescriptor::updateEllipse() {

	_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
	_rectification = std::make_shared<QGraphicsEllipseItem>(QRect(_v[0].x, _v[0].y, _v[1].x-_v[0].x, _v[1].y- _v[0].y), this);
	_rectificationMarkerOrig = std::make_shared<QGraphicsRectItem>(QRect(_v[0].x, _v[0].y, 10, 10), this);
	_rectificationMarkerEnd = std::make_shared<QGraphicsRectItem>(QRect(_v[1].x, _v[1].y, 10, 10), this);
	_rectificationMarkerOrig->setBrush(_brush);
	_rectificationMarkerEnd->setBrush(_brush);
}

EllipseDescriptor::~EllipseDescriptor()
{
}

void EllipseDescriptor::setBrush(QBrush brush) {
	_brush = brush;
	_rectificationMarkerOrig->setBrush(_brush);
	_rectificationMarkerEnd->setBrush(_brush);
}

void EllipseDescriptor::updateRect() {
	setRect(getRect());
}

void EllipseDescriptor::setRect(std::vector<cv::Point> rect) {
	if(rect.size() >= 3){
		_v = rect;
		(dynamic_cast<AreaInfoElement*>(getModel()))->setVertices(rect);
		updateEllipse();
	}
}

std::vector<cv::Point> EllipseDescriptor::getRect() {
	return (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
}


void EllipseDescriptor::getNotified() {

}

QRectF EllipseDescriptor::boundingRect() const
{
	return QRect(10, 10, 10, 10);
}

void EllipseDescriptor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!_isInit)
		init();
}

bool EllipseDescriptor::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
	return 1;
	if (event->type() == QEvent::GraphicsSceneMousePress) {

		QGraphicsRectItem *r = dynamic_cast<QGraphicsRectItem *>(watched);
		if (r && (r == _rectificationMarkerOrig.get() || r == _rectificationMarkerEnd.get())) {
				QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent*)event;
				_watchingDrag = watched;
				_dragX = e->pos().x();
				_dragY = e->pos().y();
		}		
	}
	if (event->type() == QEvent::GraphicsSceneMouseRelease) {

		QGraphicsRectItem *r = dynamic_cast<QGraphicsRectItem *>(watched);
		if (r && (r == _rectificationMarkerOrig.get() || r == _rectificationMarkerEnd.get())) {
			//Find the one which was dragged


			QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent*)event;
			int nowX = e->pos().x();
			int nowY = e->pos().y();

			if (r == _rectificationMarkerOrig.get())
				_v[0] = cv::Point(nowX, nowY);
			else
				_v[1] = cv::Point(nowX, nowY);


			(dynamic_cast<AreaInfoElement*>(getModel()))->setVertices(_v);

		///	Q_EMIT updatedPoints();

			updateEllipse();
		}
	}
	return true;
}
