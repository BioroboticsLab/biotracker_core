#include "RotationHandle.h"
#include "QPainter"
#include "QGraphicsSceneMouseEvent"
#include "QCursor"
#include "QtMath"
#include "qdebug.h"


RotationHandle::RotationHandle(QPoint origin, QGraphicsItem* parent) :
	QGraphicsObject(parent), _origin(origin)
{
	setFlag(ItemIsMovable);
	//setFlag(ItemIgnoresTransformations);
}

RotationHandle::~RotationHandle()
{
	delete this;
}

QRectF RotationHandle::boundingRect() const
{
	return QRectF(-3, -3, 6, 6);
}

void RotationHandle::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::red));

	painter->drawEllipse(QRect(-2, -2, 4, 4));

}

void RotationHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		// handle left mouse button here
		setCursor(Qt::ClosedHandCursor);
		update();
	}
	//pass on
	QGraphicsItem::mousePressEvent(event);
}

void RotationHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		// handle left mouse button here
		setCursor(Qt::ArrowCursor);

		double angleRad = atan2(this->y() - _origin.y(), this->x() - _origin.x());
		double angleDeg = qRadiansToDegrees(angleRad);

		Q_EMIT emitShapeRotation(angleDeg, true);

		update();
	}
	//pass on
	QGraphicsItem::mouseReleaseEvent(event);
}

void RotationHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{

	double angleRad = atan2(this->y() - _origin.y(), this->x() - _origin.x());
	double angleDeg = qRadiansToDegrees(angleRad);

	qDebug() << angleDeg;

	Q_EMIT emitShapeRotation(angleDeg);
	 
	update();

	//pass on
	QGraphicsItem::mouseMoveEvent(event);
}
