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
#include "Interfaces\IModel\IModelTrackedTrajectory.h"
#include "QDebug"
#include "QMenu"
#include "QAction"

class QGraphicsSceneHoverEvent;

TrackedComponentView::TrackedComponentView(QGraphicsItem *parent, IController *controller, IModel *model) :
	IViewTrackedComponent(parent, controller, model)
{

	//TrackedElement *elem = dynamic_cast<TrackedElement *>(getModel());
	m_boundingRect = QRectF(0, 0, 2040, 2040);
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::MouseButtons::enum_type::LeftButton);
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
	return true;// TrackedComponentView::eventFilter(target, event);
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
	if (permission.first == ENUMS::COREPERMISSIONS::COMPONENTADD) {
		//TODO add component in model and view
		qDebug() << "TODO add component";
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
			ComponentShape* ellipse = new ComponentShape(this, trajectory, i);
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
		for (int i = 1; i <= (all->size() - childrenCount); i++) {
			IModelTrackedTrajectory* trajectory = dynamic_cast<IModelTrackedTrajectory*>(all->getChild(all->size() - i));
			if (trajectory) {
				ComponentShape* newShape = new ComponentShape(this, trajectory, trajectory->getId());
				printf("new shape created: traj-id:%i\n",i, trajectory->getId());
				printf("all-size:%i\n", all->size());
			}
			else {
				printf("error: no trajectory -> no shape created");
			}

		}
	}
}

void TrackedComponentView::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QMenu menu;
	QAction *addComponentAction = menu.addAction("Add Component here", dynamic_cast<TrackedComponentView*>(this), SLOT(addComponent()), Qt::Key_C);
	QAction *selectedAction = menu.exec(event->screenPos());
}

void TrackedComponentView::addComponent()
{
	//ComponentShape* newShape = new ComponentShape(this, trajectory, trajectory->getId());
	qDebug() << "TODO add component";
}