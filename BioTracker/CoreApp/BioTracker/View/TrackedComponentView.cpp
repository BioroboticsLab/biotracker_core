#include "TrackedComponentView.h"
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
#include "Controller\ControllerTrackedComponentCore.h"
#include "Interfaces\IModel\IModelTrackedTrajectory.h"
#include "QDebug"
#include "QMenu"
#include "QAction"
#include "qcolordialog.h"

class QGraphicsSceneHoverEvent;

TrackedComponentView::TrackedComponentView(QGraphicsItem *parent, IController *controller, IModel *model) :
	IViewTrackedComponent(parent, controller, model)
{

	//TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());
	m_boundingRect = QRectF(0, 0, 2040, 2040);
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
	printf("Core: %i, %i", x, y);
	update();
}


QRectF TrackedComponentView::boundingRect() const
{

	return m_boundingRect;
}

void TrackedComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//printf("Core wird gezeichnet \n");
	//QGraphicsScene *scene = this->scene();
	//scene->setSceneRect(0,0,2040,2040);

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
		painter->drawText(10, 100, "No Model in Core!");
		return;
	}

	painter->drawText(0,200, "Model in core!");

	painter->setBrush(QBrush());
	painter->drawRect(this->sceneBoundingRect());
	//		QLineF angleline;
	//		angleline.setP1(QPointF(x, y));
	//		angleline.setAngle(point->getDeg());
	//		angleline.setLength(80);
	//		painter->drawLine(angleline);


	//		//create an eventhandler for each component
	//		installSceneEventFilter(this);
	//	}
	//}

	/*
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
	*/
}

void TrackedComponentView::getNotified()
{
	update();
}

/*
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

*/
bool TrackedComponentView::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {

	//if (event->type() == QEvent::GraphicsSceneMousePress) {
	//	QAbstractGraphicsShapeItem* comp = dynamic_cast<QAbstractGraphicsShapeItem*>(watched);
	//	if (comp) {
	//		comp->setBrush(QBrush(Qt::black));
	//		update();
	//	}
	//}

	/*
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
					for (int i = 0; i < all->size(); i++) {
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
	*/
	return true;
}

bool TrackedComponentView::eventFilter(QObject *target, QEvent *event)
{
	return false;// TrackedComponentView::eventFilter(target, event);
}

void TrackedComponentView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	/*
	const QPointF event_scene_pos = event->scenePos();


	TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(getModel());
	if (t) {
	for (int i = 0; i < t->size(); i++) {
	TrackingRectelement *r = dynamic_cast<TrackingRectElement *>(t->getChild(i));
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

void TrackedComponentView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	/*
	const QPointF event_scene_pos = event->scenePos();

	TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(getModel());
	if (t) {
	for (int i = 0; i < t->size(); i++) {
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
//
//void TrackedComponentView::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
//{
//	event->accept();
//}
//
//void TrackedComponentView::dropEvent(QGraphicsSceneDragDropEvent * event)
//{
//	qDebug() << "DROP EVENT";
//}

QVariant TrackedComponentView::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change == ItemSceneHasChanged) {
		createChildShapesAtStart();
	}
	return QGraphicsItem::itemChange(change, value);
}

// set permissions, which where send by the plugin
void TrackedComponentView::setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{
	m_permissions.insert(permission);
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
	// check if scene is set
	if (!(this->scene())) {
		printf("viewscene is null\n");
	}
	
	// create a shape for each model-component upon plugin-init
	IModelTrackedTrajectory *all = dynamic_cast<IModelTrackedTrajectory *>(getModel());
	for (int i = 0; i < all->size(); i++) {
		IModelTrackedTrajectory* trajectory = dynamic_cast<IModelTrackedTrajectory*>(all->getChild(i));
		if (trajectory) {
			//create componentshape for trajectory
			ComponentShape* newShape = new ComponentShape(this, trajectory, trajectory->getId());
			//QObject::connect(newShape, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)));
			//QObject::connect(newShape, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, QPoint)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveMoveElement(IModelTrackedTrajectory*, QPoint)));
			//QObject::connect(newShape, SIGNAL(broadcastMove()), this, SLOT(receiveBroadcastMove()), Qt::DirectConnection);
		}

		IModelTrackedPoint *rect = dynamic_cast<IModelTrackedPoint *>(all->getChild(i));
		if (rect) {
			int x = rect->getX();
			int y = rect->getY();
			int w = 20;
			int h = 20;

			QGraphicsRectItem* rect = new QGraphicsRectItem(x - w/2, y -h/2, w, h, this);
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


void TrackedComponentView::updateShapes(uint framenumber) {
	IModelTrackedTrajectory *all = dynamic_cast<IModelTrackedTrajectory *>(getModel());

	//qDebug() << "main:" << all->getId();

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
				//connectShape(newShape);

				QObject::connect(newShape, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)), Qt::DirectConnection);
				QObject::connect(newShape, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, QPoint)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveMoveElement(IModelTrackedTrajectory*, QPoint)), Qt::DirectConnection);
				QObject::connect(newShape, SIGNAL(broadcastMove()), this, SLOT(receiveBroadcastMove()), Qt::DirectConnection);

				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTMOVE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTMOVE]));
				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTREMOVE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTREMOVE]));
				newShape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTSWAP, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTSWAP]));

				//TODO do this in extra function
				newShape->receiveTracingLength(coreParams->m_tracingHistory);
				newShape->receiveTracingStyle(coreParams->m_tracingStyle);
				newShape->receiveTracingSteps(coreParams->m_tracingSteps);
			}
			else {
				printf("error: no trajectory -> no shape created");
			}

		}
	}
}
// gets triggered when one or more shape is moved; emits move signal to tracker for all selected shapes
// TODO: this way cause i don't see a better way to get drop-event for not clicked shapes (they dont get mouseevents if multiple shapes are selected)
void TrackedComponentView::receiveBroadcastMove()
{
	QList<QGraphicsItem *> allSelectedItems = this->scene()->selectedItems();
	
	QGraphicsItem* item;

	for each (item in allSelectedItems) {
		ComponentShape* shape = dynamic_cast<ComponentShape*>(item);
		if (shape) {
			if (shape->isSelected()) {
				IModelTrackedTrajectory* shapeTrajectory = shape->getTrajectory();
				shape->emitMoveElement(shapeTrajectory, shape->pos().toPoint());
				qDebug() << shape->pos().toPoint();
			}
		}
	}
}

void TrackedComponentView::receiveViewSwitch(bool lever)
{
	this->setVisible(lever);
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
	QMenu menu;
	QAction *addComponentAction = menu.addAction("Add Component here", dynamic_cast<TrackedComponentView*>(this), SLOT(addTrajectory()), Qt::Key_A);
	QAction *swapIdsAction = menu.addAction("Swap ID's", dynamic_cast<TrackedComponentView*>(this), SLOT(swapIds()), Qt::Key_S);
	QAction *unmarkAllAction = menu.addAction("Unmark all...", dynamic_cast<TrackedComponentView*>(this), SLOT(addTrajectory()), Qt::Key_U);
	QAction *removeSelectedAction = menu.addAction("Remove selected shapes", dynamic_cast<TrackedComponentView*>(this), SLOT(removeTrajectories()), Qt::Key_Backspace);

	
	// manage adding
	if (!m_permissions[ENUMS::COREPERMISSIONS::COMPONENTSWAP]) {
		addComponentAction->setEnabled(false);
	}

	// manage swapping
	swapIdsAction->setEnabled(false);
	QList<QGraphicsItem *> allSelectedItems = this->scene()->selectedItems();
	// check if two shapes are selected
	if (allSelectedItems.size() == 2 && dynamic_cast<ComponentShape*>(allSelectedItems[0]) && dynamic_cast<ComponentShape*>(allSelectedItems[1])) {
		ComponentShape* selectedItem0 = dynamic_cast<ComponentShape*>(allSelectedItems[0]);
		ComponentShape* selectedItem1 = dynamic_cast<ComponentShape*>(allSelectedItems[1]);
		// check if both shapes are swappable (check permission for each shape
		if (selectedItem0->isSwappable() && selectedItem1->isSwappable()) {
			swapIdsAction->setEnabled(true);
		}
	}

	// execute menu
	QAction *selectedAction = menu.exec(event->screenPos());

}

void TrackedComponentView::addTrajectory()
{
	if (!lastClickedPos.isNull()) {
		qDebug() << "new Component at position " << lastClickedPos;
		emitAddTrajectory(lastClickedPos);
		lastClickedPos = QPoint(0, 0);
	}
	else {
		qDebug() << "could not add new component";
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
	for each (item in allSelectedItems) {
		ComponentShape* shape = dynamic_cast<ComponentShape*>(item);
		if (shape) {
			shape->removeShape();
		}
	}
}

//void TrackedComponentView::connectShape(ComponentShape* shape) {
//
//	ControllerTrackedComponentCore* ctrTrCompView = dynamic_cast<ControllerTrackedComponentCore*>(this->getController());
//
//	QObject::connect(shape, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), ctrTrCompView, SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)));
//	QObject::connect(shape, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, QPoint)), ctrTrCompView, SLOT(receiveMoveElement(IModelTrackedTrajectory*, QPoint)));
//}