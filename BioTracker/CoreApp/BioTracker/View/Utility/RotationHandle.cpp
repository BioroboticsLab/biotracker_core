#include "RotationHandle.h"
#include "QPainter"
#include "QGraphicsSceneMouseEvent"
#include "QCursor"
#include "QtMath"
#include "QGraphicsScene"
#include "qdebug.h"


RotationHandle::RotationHandle(QPoint origin, QAbstractGraphicsShapeItem* parent) :
	QAbstractGraphicsShapeItem(parent), _origin(origin)
{
	setFlag(ItemIsMovable);
	setPen(QPen(Qt::black));
	setBrush(QBrush(Qt::red));
	//setFlag(QGraphicsItem::ItemIsSelectable);	
	//setFlag(ItemIgnoresTransformations);
}

RotationHandle::~RotationHandle()
{
}

QRectF RotationHandle::boundingRect() const
{
	return QRectF(-5, -5, 10, 10);
}

void RotationHandle::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if(m_antialiasing){
		painter->setRenderHint(QPainter::Antialiasing);
	}
	painter->setPen(this->pen());
	painter->setBrush(this->brush());

	painter->drawEllipse(QRect(-5, -5, 10, 10));

	//painter->drawRect(boundingRect());
}

void RotationHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		// handle left mouse button here

		//unselect all selected items so they wont get moved
		QList<QGraphicsItem *> allSelectedItems = scene()->selectedItems();
		QGraphicsItem* item;
		foreach (item, allSelectedItems) {
			item->setSelected(false);
		}


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

	Q_EMIT emitShapeRotation(angleDeg);
	 
	update();

	//pass on
	QGraphicsItem::mouseMoveEvent(event);
}

void RotationHandle::setAntialiasing(bool toggle)
{
	m_antialiasing = toggle;
	update();
}
