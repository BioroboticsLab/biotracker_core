#include "ComponentShape.h"
#include "QBrush"
#include "QPainter"
#include "QMenu"
#include "QAction"
#include "QGraphicsScene"
#include "QGraphicsSceneContextMenuEvent"
#include "QDebug"
#include "QColorDialog"

ComponentShape::ComponentShape(QGraphicsObject* parent, IModelTrackedTrajectory* trajectory, int id):
	QGraphicsObject(parent), m_trajectory(trajectory), m_id(id), m_parent(parent), m_w(20), m_h(20)
{
	setData(0, m_id);

	m_penColor = Qt::black;
	m_penWidth = 0;
	m_penStyle = Qt::SolidLine;
	m_brushColor = Qt::blue;
	m_marked = false;
	m_pMovable = true;
	m_pRemovable = true;
	m_pSwappable = true;

	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTADD, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTMOVE, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTSWAP, true));


	//IModelTrackedComponent* component = dynamic_cast<IModelTrackedComponent *>(trajectory->getLastChild());
	//printf("shape creation: traj-size: %i\n", m_trajectory->size()); 
	//IModelTrackedPoint* point = dynamic_cast<IModelTrackedPoint*>(component);

	setData(1, "point");

	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);
	setPos(0 - m_w / 2 , 0- m_h / 2);

	qDebug() << "shape is created at:" << pos();
	//for draging, disable with permissions
	setAcceptedMouseButtons(Qt::LeftButton);

}

QRectF ComponentShape::boundingRect() const
{
	return QRectF(0,0,20,20);
}

void ComponentShape::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//TODO integrate to expert options
	//painter->setRenderHint(QPainter::Antialiasing);

	//qDebug() << "shape is painted at:" << pos();


	//check if scene is set
	if (!(this->scene())) {
		printf("componentscene is null\n");
	}

	//draw ellipse
	if (this->data(1) == "point") {
		QRectF ellipse = QRectF(0,0,m_w,m_h);
		QPen pen = QPen(m_penColor, m_penWidth, m_penStyle);
		painter->setPen(pen);
		painter->setBrush(QBrush(m_brushColor));
		painter->drawEllipse(ellipse);
	}
}

QPainterPath ComponentShape::shape() const
{
	QPainterPath path;
	path.addEllipse(0,0,m_w,m_h);
	return path;
}

bool ComponentShape::advance()
{
	return false;
}

bool ComponentShape::updatePosition(uint framenumber)
{
	//printf("update a shape\n");

	//printf("traj-size: %i;  framenumber:%u\n", this->m_trajectory->size(), framenumber);

	if (!m_trajectory) {
		printf("trajectory not existant, delete child %i...\n", m_id);
		delete this;
		return false;
	}


	//printf("localtraj-size: %i\n", m_trajectory->size());
	if (m_trajectory->size() != 0) {

		//TODO for each IModelTrackedComponent type
		IModelTrackedPoint* component = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(framenumber));
		if (component) {
			// if component and traj valid -> show and update
			if (component->getValid() && m_trajectory->getValid()) {
				this->setPos(component->getX() - m_w/2, component->getY() - m_w/2);
				this->show();
				update();

				//qDebug() << "shape is updated to:" << pos();

				//printf("shape updated\n");
			}
			//TODO think about removing entirely or not
			// else hide shape
			else {
				this->hide();
				update();
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

void ComponentShape::setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{
	m_permissions.insert(permission);


	switch (permission.first)
	{
		case ENUMS::COREPERMISSIONS::COMPONENTMOVE:
			m_pMovable = permission.second;
			this->setFlag(ItemIsMovable, permission.second);
			break;
		case ENUMS::COREPERMISSIONS::COMPONENTREMOVE:
			m_pRemovable = permission.second;
			break;
		case ENUMS::COREPERMISSIONS::COMPONENTSWAP:
			m_pSwappable = permission.second;
	}

	qDebug() << "shape permission " << permission.first << "set to " << permission.second;
}

int ComponentShape::getId()
{
	return m_id;
}

bool ComponentShape::isSwappable()
{
	return m_permissions[ENUMS::COREPERMISSIONS::COMPONENTSWAP];
}

bool ComponentShape::isRemovable()
{
	return m_pRemovable;
}

void ComponentShape::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	//qDebug() << "shape get dragged at:" << pos();

	m_mousePressPos = pos().toPoint();
	//qDebug()<< "PRESS" << m_mousePressPos;

	if (event->button() == Qt::LeftButton) {
		// handle left mouse button here
		setCursor(Qt::ClosedHandCursor);
		m_dragged = true;
		update();
	}
	//pass on
	QGraphicsItem::mousePressEvent(event);
}

void ComponentShape::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		setCursor(Qt::ArrowCursor);
		m_dragged = false;
		//qDebug() << "DROP " << this->getId() << ": " << pos();
		
		// signal new position to controller
		//emitMoveElement(m_trajectory, pos().toPoint());

		//broadcast move so other selected elements get moved too
		// TODO? maybe unconventional and slow but couldn't find another way; Dropevents in view and dropevents in shape didn't seem to work
		broadcastMove();

		update();
	}
	//pass on
	QGraphicsItem::mouseReleaseEvent(event);

}

void ComponentShape::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
	//qDebug() << "shape gets moved to:" << pos();
	QGraphicsItem::mouseMoveEvent(event);
}

QVariant ComponentShape::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSelectedHasChanged && scene()) {
		qDebug() << this->getId() << " selected changed to" << value;
		if (this->m_marked) {
			this->markShape();
		}
		else {
			this->unmarkShape();
		}
	}
	return QGraphicsItem::itemChange(change, value);
}

void ComponentShape::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QMenu menu;
	QAction *changeBrushColorAction = menu.addAction("Change brush color",dynamic_cast<ComponentShape*>(this),SLOT(changeBrushColor()), Qt::SHIFT + Qt::Key_A);
	QAction *changePenColorAction = menu.addAction("Change pen color", dynamic_cast<ComponentShape*>(this), SLOT(changePenColor()), Qt::Key_X);
	
	QAction *removeAction = menu.addAction("Remove", dynamic_cast<ComponentShape*>(this), SLOT(removeShape()), Qt::Key_D);
	if (!m_pRemovable) { removeAction->setEnabled(false); }

	QAction *markAction = menu.addAction("Mark", dynamic_cast<ComponentShape*>(this), SLOT(markShape()), Qt::Key_M);
	QAction *unmarkAction = menu.addAction("Unmark", dynamic_cast<ComponentShape*>(this), SLOT(unmarkShape()), Qt::Key_M);
	if (m_marked) {
		markAction->setEnabled(false);
		unmarkAction->setEnabled(true);
	}
	else {
		markAction->setEnabled(true);
		unmarkAction->setEnabled(false);
	}



	if (this->parentObject()->isVisible()) {
		qDebug() << "Parent is visible";
	}

	if (this->parentObject()->isActive()) {
		qDebug() << "Parent is active";
	}
	if (this->parentObject()->hasFocus()) {
		qDebug() << "Parent has focus";
	}
	
	QAction *selectedAction = menu.exec(event->screenPos());
}

//SLOTS

void ComponentShape::changeBrushColor()
{
	this->m_brushColor = QColorDialog::getColor();
	update();
}

void ComponentShape::changePenColor()
{
	this->m_penColor = QColorDialog::getColor();
	update();
}

bool ComponentShape::removeShape()
{
	if (m_pRemovable) {
		qDebug() << "Removing shape...";

		//emit to set trajectory invalid 
		emitRemoveTrajectory(m_trajectory);
		//hide this shape
		this->hide();
	}
	else {
		qDebug() << "component shape is not removable";
	}
	return m_pRemovable;
}

void ComponentShape::markShape(int penwidth)
{	
	m_marked = true;
	m_penWidth = penwidth;
	if (this->isSelected()) {
		m_penColor = Qt::red;
		m_penStyle = Qt::DashLine;
	}
	else {
		m_penColor = Qt::black;
		m_penStyle = Qt::SolidLine;
	}
	update();
}

void ComponentShape::unmarkShape()
{
	m_marked = false;
	m_penWidth = 0;
	if (this->isSelected()) {
		m_penColor = Qt::red;
		m_penStyle = Qt::DashLine;
	}
	else {
		m_penColor = Qt::black;
		m_penStyle = Qt::SolidLine;
	}
	update();
}
