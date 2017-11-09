#include "ComponentShape.h"
#include "QBrush"
#include "QPainter"
#include "QMenu"
#include "QAction"
#include "QGraphicsScene"
#include "QGraphicsSceneContextMenuEvent"
#include "QDebug"
#include "QColorDialog"
#include <assert.h>

ComponentShape::ComponentShape(QGraphicsObject* parent, IModelTrackedTrajectory* trajectory, int id):
	QGraphicsObject(parent), m_trajectory(trajectory), m_id(id), m_parent(parent), m_w(20), m_h(20)
{
	setData(0, m_id);

	m_penColor = Qt::black;
	m_penWidth = 0;
	m_penStyle = Qt::SolidLine;
	m_brushColor = Qt::blue;
	m_marked = false;
	m_tracingStyle = "None";
	m_tracingLength = 0;
	m_pMovable = true;
	m_pRemovable = true;
	m_pSwappable = true;
	m_currentFramenumber = 0;
	m_tracingSteps = 1;

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

    //TODO Jonas check if these are always sane
    assert(m_tracingSteps > 0);
    int tracingSteps = std::max(m_tracingSteps, 1);
    if (m_currentFramenumber <= 0)
        return;

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
		if (m_tracingStyle == "None" || m_tracingLength == 0) {
			return;
		}
		
		if (m_trajectory->size() != 0) {
			//instant tracing (no saved history)
			//TODO don't do this in paint()! or do you?
			if (m_tracingLength > 0) {

				IModelTrackedPoint* currentChild = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(m_currentFramenumber));
				QPoint currentPoint = QPoint(currentChild->getXpx(), currentChild->getYpx());

				QPoint lastPointDifference = QPoint(0,0) + QPoint(m_h/2, m_w/2);

				for (int i = 1; i <= m_tracingLength; i+= tracingSteps) {
					qDebug() << this->getId();
					if (i <= m_currentFramenumber) {

						IModelTrackedPoint* historyChild = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(m_currentFramenumber - i));
						if (historyChild) {
							//positioning
							QPoint historyPoint = QPoint(historyChild->getXpx(), historyChild->getYpx());
							QPoint historyPointDifference = historyPoint - currentPoint;
							QPoint adjustedHistoryPointDifference = historyPointDifference + QPoint(m_w / 2, m_h / 2);
							
							// TODO draw actually the shapes not just ellipses
							if (m_tracingStyle == "Shape") {
								QRectF traceEllipse = QRectF(historyPointDifference.x() + m_w/4, historyPointDifference.y() + m_h/4, m_w / 2, m_h / 2);

								//drawing
								QColor opaquePenColor = QColor(m_penColor.red(), m_penColor.green(), m_penColor.blue(), (255 - (int)(255 / m_tracingLength) * i) + 0.01);
								QPen opaquePen = QPen(opaquePenColor, m_penWidth, Qt::SolidLine);
								QColor opaqueBrushColor = QColor(m_brushColor.red(), m_brushColor.green(), m_brushColor.blue(), (int)(255 - (255 / m_tracingLength) * i) + 0.01);
								QBrush opaqueBrush = QBrush(opaqueBrushColor);
								painter->setPen(opaquePen);
								painter->setBrush(opaqueBrush);
								painter->drawEllipse(traceEllipse);
							}
							else if (m_tracingStyle == "Path") {
								//QLineF arrow[2];
								
								QLineF base = QLineF(adjustedHistoryPointDifference, lastPointDifference);
								//QLineF arm0 = QLineF();
								//arm0.setP2(lastPointDifference);
								//arm0.setP1(QPoint(lastPointDifference.x() + 10, lastPointDifference.y() + 10));
								//arm0.setAngle(base.angle() + 20);

								//arrow[0] = base;
								//arrow[1] = arm0;

								//painter->drawLines(arrow, 2);

								painter->drawLine(base);

								lastPointDifference = adjustedHistoryPointDifference;
							}
							else if (m_tracingStyle == "ArrowPath") {
								//QLineF arrow[2];
								QLineF base = QLineF(adjustedHistoryPointDifference, lastPointDifference);
								QLineF arm0 = base.normalVector();
								arm0.setLength(10);
								arm0.setAngle(base.angle() -160);

								QLineF arm1 = base.normalVector();
								arm1.setLength(10);
								arm1.setAngle(base.angle() - 200);

								painter->drawLine(base);
								painter->drawLine(arm0);
								painter->drawLine(arm1);

								lastPointDifference = adjustedHistoryPointDifference;
							}
						}
					}
				}
			}
		}
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
	m_currentFramenumber = framenumber;

	if (!m_trajectory) {
		//printf("trajectory not existant, delete child %i...\n", m_id);
		delete this;
		return false;
	}


	//printf("localtraj-size: %i\n", m_trajectory->size());
    if (m_trajectory->size() != 0 && m_trajectory->getValid()) {

		//TODO for each IModelTrackedComponent type
		IModelTrackedPoint* component = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(framenumber));
		if (component) {
			// if component and traj valid -> show and update
			if (component->getValid() && m_trajectory->getValid()) {
				this->setPos(component->getXpx() - m_w/2, component->getYpx() - m_w/2);
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
			//printf("no tracked component in this trajectory in frame: %u", framenumber);
		}	
	}
	else {
		//printf("trajectory empty, delete child%i...\n", m_id);
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
	QAction *changeBrushColorAction = menu.addAction("Change fill color",dynamic_cast<ComponentShape*>(this),SLOT(changeBrushColor()), Qt::SHIFT + Qt::Key_A);
	QAction *changePenColorAction = menu.addAction("Change border color", dynamic_cast<ComponentShape*>(this), SLOT(changePenColor()), Qt::Key_X);
	
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

void ComponentShape::changeBrushColor(QColor color)
{
	m_brushColor = color;
	update();
}

void ComponentShape::changePenColor(QColor color)
{
	m_penColor = color;
	update();
}

bool ComponentShape::removeShape()
{
	if (m_pRemovable) {
		qDebug() << "Removing shape...";

		//emit to set trajectory invalid 
		Q_EMIT emitRemoveTrajectory(m_trajectory);
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

void ComponentShape::receiveTracingLength(int tracingLength)
{
	m_tracingLength = tracingLength;
	update();
}

void ComponentShape::receiveTracingStyle(QString style)
{
	m_tracingStyle = style;
	update();
}

void ComponentShape::receiveTracingSteps(int steps)
{
	m_tracingSteps = steps;
	update();
}
