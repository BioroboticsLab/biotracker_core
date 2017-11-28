#include "TrackedComponentView.h"
#include <assert.h>
//#include "Model/TrackedComponents/TrackedElement.h"
//#include "Model/TrackedComponents/TrackingRectElement.h"
//#include "Model/TrackedComponents/TrackedTrajectory.h"
//#include "Model/TrackingAlgorithm/property/Rectification.h"
#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>
#include "QGraphicsScene"
#include "QGraphicsEllipseItem"

#include "ComponentShape.h"
#include "Model/CoreParameter.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "QDebug"
#include "QMenu"
#include "QAction"
#include "qcolordialog.h"

class QGraphicsSceneHoverEvent;

TrackedComponentView::TrackedComponentView(QGraphicsItem *parent, IController *controller, IModel *model) :
	IViewTrackedComponent(parent, controller, model)
{

	//TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());
	m_boundingRect = QRectF(0, 0, 4080, 4080);
    m_currentFrameNumber = 0;
	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	//setAcceptedMouseButtons(Qt::MouseButtons::enum_type::LeftButton);
	_watchingDrag = 0;

	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTVIEW, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTADD, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTMOVE, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTREMOVE, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTSWAP, true));
}

void TrackedComponentView::rcvDimensionUpdate(int x, int y) {
	m_boundingRect = QRectF(0, 0, x, y);
	update();
}


QRectF TrackedComponentView::boundingRect() const
{

	return m_boundingRect;
}

void TrackedComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	IModelTrackedTrajectory *all = dynamic_cast<IModelTrackedTrajectory *>(getModel());

	//check if new components were added to the model

	
	QFont* font = new QFont();
	font->setPointSize(50);

	painter->setFont(*font);

	QBrush brush(Qt::red);
	painter->setBrush(brush);
	painter->setPen(QPen(Qt::red, 5));

	// check if model exists

	if (!all) {
		//TODO Jonas debug
		//painter->drawText(10, 100, "No Model in Core!");
		return;
	}

	//TODO Jonas debug
	//painter->drawText(0,200, "Model in core!");

	painter->setBrush(QBrush());
	//TODO Jonas debug
	//painter->drawRect(this->sceneBoundingRect());
}

void TrackedComponentView::getNotified()
{
	updateShapes(m_currentFrameNumber);
	update();
}

bool TrackedComponentView::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
	return true;
}

bool TrackedComponentView::eventFilter(QObject *target, QEvent *event)
{
	return false;// TrackedComponentView::eventFilter(target, event);
}

void TrackedComponentView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
}

void TrackedComponentView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
}

QVariant TrackedComponentView::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change == ItemSceneHasChanged && this->scene()) {
		createChildShapesAtStart();
	}
	return QGraphicsItem::itemChange(change, value);
}

// set permissions, which where send by the plugin
void TrackedComponentView::setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{
	m_permissions[permission.first] = permission.second;
	qDebug() << "permission " << permission.first << " set to" << permission.second;


	//first check if permission is for view, if not pass permission to shapes -> view has all permissions, shapes only certain ones
	if (permission.first == ENUMS::COREPERMISSIONS::COMPONENTVIEW) {
		this->setVisible(permission.second);
		return;
	}
	//does not need to be propagated to shapes; only handled by view
	//TODO: move this to controller
	if (permission.first == ENUMS::COREPERMISSIONS::COMPONENTADD) {
		return;
	}

	QGraphicsItem* child;
	foreach(child, this->childItems()) {
		ComponentShape* shape = dynamic_cast<ComponentShape*>(child);
		if (shape) {
			shape->setPermission(permission);
		}
	}
}


// at plugin load, draw each element of initial main trajectory
void TrackedComponentView::createChildShapesAtStart() {
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(dynamic_cast<ControllerTrackedComponentCore*>(getController())->getCoreParameter());

	// check if scene is set
    assert(this->scene());
	
	// create a shape for each model-component upon plugin-init
	IModelTrackedTrajectory *all = dynamic_cast<IModelTrackedTrajectory *>(getModel());
	if (all) {
		for (int i = 0; i < all->size(); i++) {
			IModelTrackedTrajectory* trajectory = dynamic_cast<IModelTrackedTrajectory*>(all->getChild(i));
			if (trajectory) {
				//create componentshape for trajectory
				ComponentShape* newShape = new ComponentShape(this, trajectory, trajectory->getId());
				QObject::connect(newShape, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)), Qt::DirectConnection);
				QObject::connect(newShape, SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveRemoveTrackEntity(IModelTrackedTrajectory*)), Qt::DirectConnection);
				QObject::connect(newShape, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, QPoint, int)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveMoveElement(IModelTrackedTrajectory*, QPoint, int)), Qt::DirectConnection);
				QObject::connect(newShape, SIGNAL(broadcastMove()), this, SLOT(receiveBroadcastMove()), Qt::DirectConnection);

				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTMOVE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTMOVE]));
				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTREMOVE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTREMOVE]));
				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTSWAP, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTSWAP]));

				newShape->setMembers(coreParams);
				
				//TODO bad code clean up
				//IModelTrackedPoint* point = dynamic_cast<IModelTrackedPoint*>(trajectory->getLastChild());
				//if (point) { newShape->setData(1, "point"); }
				//IModelTrackedEllipse* ellipse = dynamic_cast<IModelTrackedEllipse*>(trajectory->getLastChild());
				//if (ellipse) { newShape->setData(1, "ellipse"); }
				//IModelTrackedRectangle* rectangle = dynamic_cast<IModelTrackedRectangle*>(trajectory->getLastChild());
				//if (rectangle) { newShape->setData(1, "rectangle"); }
				//IModelTrackedPolygon* polygon = dynamic_cast<IModelTrackedPolygon*>(trajectory->getLastChild());
				//if (polygon) { newShape->setData(1, "polygon"); }

				newShape->setData(1, "ellipse");
				newShape->update();

			}

			IModelTrackedPoint *rect = dynamic_cast<IModelTrackedPoint *>(all->getChild(i));
			if (rect) {
				int x = rect->getX();
				int y = rect->getY();
				int w = 20;
				int h = 20;

				QGraphicsRectItem* rect = new QGraphicsRectItem(x - w / 2, y - h / 2, w, h, this);
				rect->setPen(QPen(Qt::blue));
				rect->setBrush(QBrush(Qt::blue));
				rect->setFlag(ItemIsMovable);
				rect->setFlag(ItemIsSelectable);
				rect->setFlag(ItemSendsGeometryChanges);
				//rect->setFlag(ItemIgnoresTransformations);
				rect->setData(0, i);

			}

		}
	}
	
}


void TrackedComponentView::updateShapes(uint framenumber) {
	m_currentFrameNumber = framenumber;

	IModelTrackedTrajectory *all = dynamic_cast<IModelTrackedTrajectory *>(getModel());
	if (!all)
		return;

	//qDebug() << "all traj id's";
	//for (int k = 0; k < all->size(); k++) {
	//	//qDebug() << dynamic_cast<IModelTrackedTrajectory*>(all->getChild(k))->getId();
	//	if (dynamic_cast<IModelTrackedTrajectory*>(all->getChild(k))->getId() != dynamic_cast<IModelTrackedPoint*>(dynamic_cast<IModelTrackedTrajectory*>(all->getChild(k))->getLastChild())->getId()) {
	//		qDebug() << "traj and element not same id!!!!!";
	//	}
	//}
	//qDebug() << "all shape traj id's";
	//for (int l = 0; l < this->childItems().size(); l++) {
	//	//qDebug() << dynamic_cast<IModelTrackedTrajectory*>(dynamic_cast<ComponentShape*>(this->childItems()[l])->getTrajectory())->getId();
	//}
	//qDebug() << "all shape id's";
	//for (int m = 0; m < this->childItems().size(); m++) {
	//	//qDebug() << dynamic_cast<ComponentShape*>(this->childItems()[m])->getId();
	//}

	//update each shape; shape deletes itself if trajectory is empty or not existant
	for (int i = 0; i < this->childItems().size(); i++) {
		ComponentShape* shape = dynamic_cast<ComponentShape*>(this->childItems()[i]);
		if (shape) {
			if (!(shape->updatePosition(framenumber))) {
				i--;
			}
		}
	}
	// check for new trajectories; for each create a new shape
	if (this->childItems().size() < all->size()) {
		int childrenCount = this->childItems().size();
		for (int i = all->size() - childrenCount; i > 0; i--) {
			IModelTrackedTrajectory* trajectory = dynamic_cast<IModelTrackedTrajectory*>(all->getChild(all->size() - i));
			if (trajectory) { 
				CoreParameter* coreParams = dynamic_cast<CoreParameter*>(dynamic_cast<ControllerTrackedComponentCore*>(getController())->getCoreParameter());

				ComponentShape* newShape = new ComponentShape(this, trajectory, trajectory->getId());
				//set type

				//TODO bad code clean up
				IModelTrackedPoint* point = dynamic_cast<IModelTrackedPoint*>(trajectory->getLastChild());
				if (point) { newShape->setData(1, "point"); }
				IModelTrackedEllipse* ellipse = dynamic_cast<IModelTrackedEllipse*>(trajectory->getLastChild());
				if (ellipse) { newShape->setData(1, "ellipse"); }
				IModelTrackedRectangle* rectangle = dynamic_cast<IModelTrackedRectangle*>(trajectory->getLastChild());
				if (rectangle) { newShape->setData(1, "rectangle"); }
				IModelTrackedPolygon* polygon = dynamic_cast<IModelTrackedPolygon*>(trajectory->getLastChild());
				if (polygon) { newShape->setData(1, "polygon"); }

				//connectShape(newShape);

				//TODO do this in extra function
				QObject::connect(newShape, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)), Qt::DirectConnection);
				QObject::connect(newShape, SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveRemoveTrackEntity(IModelTrackedTrajectory*)), Qt::DirectConnection);
				QObject::connect(newShape, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, QPoint, int)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveMoveElement(IModelTrackedTrajectory*, QPoint, int)), Qt::DirectConnection);
				QObject::connect(newShape, SIGNAL(broadcastMove()), this, SLOT(receiveBroadcastMove()), Qt::DirectConnection);

				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTMOVE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTMOVE]));
				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTREMOVE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTREMOVE]));
				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTSWAP, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTSWAP]));

				newShape->m_currentFramenumber = m_currentFrameNumber;

				newShape->setMembers(coreParams);
				newShape->updatePosition(m_currentFrameNumber);
			}
			else {
				qDebug() << "error: no trajectory -> no shape created";
			}

		}
	}
}
// gets triggered when one or more shape is moved; emits move signal to tracker for all selected shapes
// TODO this way cause i don't see a better way to get drop-event for not clicked shapes (they dont get mouseevents if multiple shapes are selected)
void TrackedComponentView::receiveBroadcastMove()
{
	QList<QGraphicsItem *> allSelectedItems = this->scene()->selectedItems();
	QList<ComponentShape*> allSelectedEntitiesList;

	
	QGraphicsItem* item;

	foreach (item, allSelectedItems) {
		ComponentShape* shape = dynamic_cast<ComponentShape*>(item);
		if (shape) {
			if (shape->isSelected()) {
				allSelectedEntitiesList.append(shape);
			}
		}
	}

	int numberSelectedEntitiesToMove = allSelectedEntitiesList.size();
	ComponentShape* entity;
	foreach(entity, allSelectedEntitiesList) {
		ComponentShape* shape = dynamic_cast<ComponentShape*>(entity);
		if (shape) {
			if (shape->isSelected()) {
				IModelTrackedTrajectory* shapeTrajectory = shape->getTrajectory();
				shape->emitMoveElement(shapeTrajectory, shape->pos().toPoint() + QPoint(shape->m_w / 2, shape->m_h / 2), numberSelectedEntitiesToMove);
				numberSelectedEntitiesToMove--;
			}
		}
	}
}


//TODO make area descriptor invisible too
void TrackedComponentView::receiveViewSwitch(bool lever)
{
	this->setVisible(lever);
}

void TrackedComponentView::receiveTrackDimensionsAll(int width, int height)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveDimensions(width, height);
		}
	}
}

void TrackedComponentView::receiveTrackDimensionsSelected(int width, int height)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape && childShape->isSelected()) {
			childShape->receiveDimensions(width, height);
		}
	}
}

void TrackedComponentView::receiveTrackOrientationLine(bool toggle)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveToggleOrientationLine(toggle);
		}
	}
}

void TrackedComponentView::receiveTrackDimensionsSetDefault()
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->setDimensionsToDefault();
		}
	}
}

void TrackedComponentView::receiveTracingSteps(int steps)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveTracingSteps(steps);
		}
	}
}

void TrackedComponentView::receiveTracingStyle(QString style)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveTracingStyle(style);
		}
	}
}

void TrackedComponentView::receiveTracingHistoryLength(int history)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveTracingLength(history);
		}
	}
}

void TrackedComponentView::receiveTracingTimeDegradation(QString degradation)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveTracingTimeDegradation(degradation);
		}
	}
}

//TODO use signals not direct calls
void TrackedComponentView::receiveColorChangeBrushAll()
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	QColor color = QColorDialog::getColor();
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->changeBrushColor(color);
		}
	}
}

//TODO use signals not direct calls
void TrackedComponentView::receiveColorChangeBorderAll()
{	
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	QColor color = QColorDialog::getColor();
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->changePenColor(color);
		}
	}
}

void TrackedComponentView::receiveColorChangeBorderSelected()
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	QColor color = QColorDialog::getColor();
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape && childShape->isSelected()) {
			childShape->changePenColor(color);
		}
	}
}

void TrackedComponentView::receiveColorChangeBrushSelected()
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	QColor color = QColorDialog::getColor();
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape && childShape->isSelected()) {
			childShape->changeBrushColor(color);
		}
	}
}

void TrackedComponentView::receiveSelectAll()
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		if (dynamic_cast<ComponentShape*>(childItem)) {
			childItem->setSelected(true);
		}
	}
}

void TrackedComponentView::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	// right clicking the view unselects all selected items so do this to avoid this behaviour
	if (event->button() != Qt::LeftButton) {
		event->accept();
		return;
	}
	QGraphicsItem::mousePressEvent(event);
}

void TrackedComponentView::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{

	lastClickedPos = event->pos().toPoint();
	//qDebug() << "rechtsclick: " << lastClickedPos;
	QMenu menu;
	QAction *addComponentAction = menu.addAction("Add entity here", dynamic_cast<TrackedComponentView*>(this), SLOT(addTrajectory()), Qt::Key_A);
	QAction *swapIdsAction = menu.addAction("Swap ID's", dynamic_cast<TrackedComponentView*>(this), SLOT(swapIds()), Qt::Key_S);
	QAction *unmarkAllAction = menu.addAction("Unmark all...", dynamic_cast<TrackedComponentView*>(this), SLOT(unmarkAll()), Qt::Key_U);
	QAction *removeSelectedAction = menu.addAction("Remove selected tracks", dynamic_cast<TrackedComponentView*>(this), SLOT(removeTrajectories()), Qt::Key_Backspace);

	
	// manage permissions
	addComponentAction->setEnabled(m_permissions[ENUMS::COREPERMISSIONS::COMPONENTADD]);
	swapIdsAction->setEnabled(m_permissions[ENUMS::COREPERMISSIONS::COMPONENTSWAP]);
	removeSelectedAction->setEnabled(m_permissions[ENUMS::COREPERMISSIONS::COMPONENTREMOVE]);

	// manage swapping
	swapIdsAction->setEnabled(false);
	QList<QGraphicsItem *> allSelectedItems = this->scene()->selectedItems();
	// check if two shapes are selected
	if (allSelectedItems.size() == 2 && dynamic_cast<ComponentShape*>(allSelectedItems[0]) && dynamic_cast<ComponentShape*>(allSelectedItems[1])) {
		ComponentShape* selectedItem0 = dynamic_cast<ComponentShape*>(allSelectedItems[0]);
		ComponentShape* selectedItem1 = dynamic_cast<ComponentShape*>(allSelectedItems[1]);
		// check if both shapes are swappable (check permission for each shape) and general permission
		if (selectedItem0->isSwappable() && selectedItem1->isSwappable() && m_permissions[ENUMS::COREPERMISSIONS::COMPONENTSWAP]) {
			swapIdsAction->setEnabled(true);
		}
	}

	// execute menu
	QAction *selectedAction = menu.exec(event->screenPos());
	lastClickedPos = QPoint(0, 0);
}

void TrackedComponentView::addTrajectory()
{
	if (!lastClickedPos.isNull()) {
		qDebug() << "TCV: new track at position " << lastClickedPos;
		emitAddTrajectory(lastClickedPos);
		lastClickedPos = QPoint(0, 0);
	}
	else {
		qDebug() << "TCV: new track at position (0,0)";
		emitAddTrajectory(QPoint(0, 0));
	}
}

void TrackedComponentView::swapIds()
{
	QList<QGraphicsItem *> allSelectedItems = this->scene()->selectedItems();
	ComponentShape* shape0 = dynamic_cast<ComponentShape*>(allSelectedItems[0]);
	ComponentShape* shape1 = dynamic_cast<ComponentShape*>(allSelectedItems[1]);
	IModelTrackedTrajectory* trajectory0 = dynamic_cast<IModelTrackedTrajectory*>(shape0->getTrajectory());
	IModelTrackedTrajectory* trajectory1 = dynamic_cast<IModelTrackedTrajectory*>(shape1->getTrajectory());

	emitSwapIds(trajectory0, trajectory1);
}

void TrackedComponentView::removeTrajectories()
{
	QList<QGraphicsItem *> allSelectedItems = this->scene()->selectedItems();
	QGraphicsItem* item;
	foreach (item, allSelectedItems) {
		ComponentShape* shape = dynamic_cast<ComponentShape*>(item);
		if (shape) {
			shape->removeTrack();
		}
	}
}

void TrackedComponentView::unmarkAll() {
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->unmarkShape();
		}
	}
}

//void TrackedComponentView::connectShape(ComponentShape* shape) {
//
//}

void TrackedComponentView::receiveTracerProportions(float proportion)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveTracerProportions(proportion);
		}
	}
}

void TrackedComponentView::receiveTracerOrientationLine(bool toggle)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveTracerOrientationLine(toggle);
		}
	}
}

void TrackedComponentView::receiveToggleAntialiasing(bool toggle)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveAntialiasing(toggle);
		}
	}
}

void TrackedComponentView::receiveIgnoreZoom(bool toggle)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveIgnoreZoom(toggle);
		}
	}
}