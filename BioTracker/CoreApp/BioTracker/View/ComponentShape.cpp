#include "ComponentShape.h"
#include "QBrush"
#include "QPainter"

ComponentShape::ComponentShape(QGraphicsObject* parent, IModelTrackedTrajectory* trajectory, int id):
	QGraphicsObject(parent), m_trajectory(trajectory), m_id(id), m_parent(parent), m_w(20), m_h(20)
{
	setData(0, m_id);

	//IModelTrackedComponent* component = dynamic_cast<IModelTrackedComponent *>(trajectory->getLastChild());
	//printf("shape creation: traj-size: %i\n", m_trajectory->size()); 
	//IModelTrackedPoint* point = dynamic_cast<IModelTrackedPoint*>(component);

		setData(1, "point");

		setFlag(ItemIsMovable);
		setFlag(ItemIsSelectable);
		setFlag(ItemSendsGeometryChanges);
		setPos(0 - m_w / 2 , 0- m_h / 2);

}

QRectF ComponentShape::boundingRect() const
{
	return QRectF(0,0,20,20);
}

void ComponentShape::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//check if scene is set
	if (!(this->scene())) {
		printf("componentscene is null\n");
	}

	//draw ellipse
	if (this->data(1) == "point") {
		QRectF ellipse = QRectF(0,0,20,20);
		painter->setPen(QPen(Qt::green));
		painter->setBrush(QBrush(Qt::blue));
		painter->drawEllipse(ellipse);
	}
}

//QPainterPath ComponentShape::shape() const
//{
//	QPainterPath path;
//	path.addEllipse(0,0,20,20);
//	return path;
//}

bool ComponentShape::advance()
{
	return false;
}

bool ComponentShape::updatePosition(uint framenumber)
{
	printf("update a shape\n");

	printf("traj-size: %i;  framenumber:%u\n", this->m_trajectory->size(), framenumber);

	if (!m_trajectory) {
		printf("trajectory not existant, delete child %i...\n", m_id);
		delete this;
		return false;
	}

	//printf("localtraj-size: %i\n", m_trajectory->size());
	if (m_trajectory->size() != 0) {

		//IModelTrackedPoint* component = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(framenumber));
		IModelTrackedPoint* component = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getLastChild());
		if (component) {
			if (component->getValid()) {
				this->setPos(component->getX() - 10, component->getY() - 10);
				this->show();
				update();
				//printf("shape updated\n");
			}
			else {
				this->hide();
			}
			return true;
		}
		else {
			printf("no tracked component in this trajectory in frame: %u", framenumber);
		}
		
	}
	else {
		printf("trajectory empty, delete child%i...\n", m_id);
		delete this;
		return false;
	}

}

IModelTrackedTrajectory * ComponentShape::getTrajectory()
{
	return m_trajectory;
}

QVariant ComponentShape::itemChange(GraphicsItemChange change, const QVariant & value)
{
	return QGraphicsItem::itemChange(change, value);;
}

void ComponentShape::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	update();
	QGraphicsItem::mousePressEvent(event);
}

void ComponentShape::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	update();
	QGraphicsItem::mouseReleaseEvent(event);
}
