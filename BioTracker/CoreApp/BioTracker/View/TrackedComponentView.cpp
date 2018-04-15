#include "TrackedComponentView.h"
#include <assert.h>

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
#include "qlabel.h"
#include "QWidgetAction"
#include "QAction"
#include "qcolordialog.h"

class QGraphicsSceneHoverEvent;

TrackedComponentView::TrackedComponentView(QGraphicsItem *parent, IController *controller, IModel *model) :
	IViewTrackedComponent(parent, controller, model)
{

	m_boundingRect = QRectF(0, 0, 4080, 4080);
    m_currentFrameNumber = 0;
	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	_watchingDrag = 0;

	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTVIEW, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTADD, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTMOVE, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTREMOVE, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTSWAP, true));
	m_permissions.insert(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTROTATE, true));
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

	// check if model exists
	if (!all) {
		return;
	}
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
	return false;
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

// set permissions, which were send by the plugin
void TrackedComponentView::setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{
	m_permissions[permission.first] = permission.second;
	qDebug() << permission.first << " set to" << permission.second;


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

	//flush all old children (e.g. from previous trackers)
	foreach(QGraphicsItem* child, this->childItems()) {
		child->hide();
		delete child;
	}

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
				connectShape(newShape);
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
				rect->setData(0, i);

			}

		}
	}
	
}


void TrackedComponentView::updateShapes(uint framenumber) {
	m_currentFrameNumber = framenumber;

	IModelTrackedTrajectory *all = dynamic_cast<IModelTrackedTrajectory *>(getModel());
	if (!all || all->size() == 0){
		//if root is nullptr, delete all children
		foreach (auto child, this->childItems()){
			delete child;
		}
		return;
	}
    
	//update each shape; shape hides itself if trajectory is empty or not existant or currentchild 
	foreach(auto child, this->childItems()){
		ComponentShape* shape = dynamic_cast<ComponentShape*>(child);
		if(shape){
			shape->updateAttributes(m_currentFrameNumber);
		}
	}
	// check for new trajectories; for each create a new shape

	//if #shapes smaller than #validtracks add new shapes
	//int validTracks = all->validCount();

	// if (this->childItems().size() < validTracks) {
	// 	int childrenCount = this->childItems().size();
	// 	// iterate over trajectories form back to increase performance
	// 	for (int i = all->size()-1; i >= 0 && this->childItems().size() < validTracks; i--) {
	// 		IModelTrackedTrajectory* trajectory = dynamic_cast<IModelTrackedTrajectory*>(all->getChild(i));
	// 		//trajectory must not be null, must be valid, and must not be already visualized
	// 		if (trajectory && trajectory->getValid() && !checkTrajectory(trajectory)) {
	// 			ComponentShape* newShape = new ComponentShape(this, trajectory, trajectory->getId());
	// 			connectShape(newShape);
	// 		}
	// 		else {
	// 			//qDebug() << "error: no trajectory valid -> no shape created";
	// 		}
	// 	}
	// }

	if (this->childItems().size() < all->size()){
		//iterate over trajectories from back to increase performance
		for (int i = all->size()-1; i >= 0 && this->childItems().size() < all->size(); i--) {
			IModelTrackedTrajectory* trajectory = dynamic_cast<IModelTrackedTrajectory*>(all->getChild(i));
			//check if trajectory already has shape object
			if(trajectory && !checkTrajectory(trajectory)){
				ComponentShape* newShape = new ComponentShape(this, trajectory, trajectory->getId());
	 			connectShape(newShape);
			}
			else{
				//already there or nullptr
			}
			//all trajectories already have shapes
			if(all->size() == this->childItems().size()){
				continue;
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

	foreach(item, allSelectedItems) {
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
				shape->emitMoveElement(shapeTrajectory,shape->getOldPos() + QPoint(shape->m_w / 2, shape->m_h / 2), shape->pos().toPoint() + QPoint(shape->m_w / 2, shape->m_h / 2), m_currentFrameNumber, numberSelectedEntitiesToMove);
				numberSelectedEntitiesToMove--;
			}
		}
	}
}

//TODO make area descriptor invisible too
void TrackedComponentView::receiveViewSwitch(bool lever)
{
	this->setVisible(lever);
	
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->m_tracingLayer->setVisible(lever);
		}
	}
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

void TrackedComponentView::receiveTrackShowId(bool toggle)
{
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveShowId(toggle);
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
    const QColorDialog::ColorDialogOptions options = QFlag(QColorDialog::ShowAlphaChannel || QColorDialog::DontUseNativeDialog);
    QColor color = QColorDialog::getColor(Qt::white, nullptr, QString("Choose fill color"), options);
//    QColorDialog* cDialog = new QColorDialog(Qt::white);
//    cDialog->show();
//    cDialog->activateWindow();
//    cDialog->raise();
//    cDialog->setFocus();

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
    QColor color = QColorDialog::getColor(Qt::white, nullptr, QString("Choose border color"), QColorDialog::ShowAlphaChannel);;
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
	QColor color = QColorDialog::getColor(Qt::white, nullptr, QString("Choose border color"), QColorDialog::ShowAlphaChannel);;
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
	QColor color = QColorDialog::getColor(Qt::white, nullptr, QString("Choose fill color"), QColorDialog::ShowAlphaChannel);;
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

	QMenu menu;


	// create the info box
	QWidgetAction* infoBox = new QWidgetAction(this);
	QString info = QString("Position (x,y) : ");
	info.append(QString("(" + QString::number(lastClickedPos.x()) + ", " + QString::number(lastClickedPos.y()) + ")"));
	QLabel* infoLabel = new QLabel(info);
	infoLabel->setWordWrap(true);
	infoLabel->setStyleSheet("QLabel {font-weight: bold; text-align: center}");
	infoLabel->setAlignment(Qt::AlignCenter);
	infoBox->setDefaultWidget(infoLabel);
	menu.addAction(infoBox);
	menu.addSeparator();
	QAction *addComponentAction = menu.addAction("Add trajectory here", dynamic_cast<TrackedComponentView*>(this), SLOT(addTrajectory()));
	QAction *swapIdsAction = menu.addAction("Swap ID's", dynamic_cast<TrackedComponentView*>(this), SLOT(swapIds()));
	QAction *unmarkAllAction = menu.addAction("Unmark all...", dynamic_cast<TrackedComponentView*>(this), SLOT(unmarkAll()));
	QAction *removeSelectedAction = menu.addAction("Remove selected trajectories", dynamic_cast<TrackedComponentView*>(this), SLOT(removeTrajectories()));

	
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
	IModelTrackedTrajectory *all = dynamic_cast<IModelTrackedTrajectory *>(getModel());
	int id = -1;
	if (all) {
		id = all->size()+1;
	}

	if (id <= 0) {
		qWarning() << "ID cannot be smaller than or equal 0";
	}

	if (!lastClickedPos.isNull()) {
		//qDebug() << "TCV: new track at position " << lastClickedPos;
		emitAddTrajectory(lastClickedPos, id);
		lastClickedPos = QPoint(0, 0);
	}
	else {
		//qDebug() << "TCV: new track at center of top left quarter of video";
		QPoint topLeftQuarterCenter = QPoint(this->boundingRect().width() / 8, this->boundingRect().height() / 8);
		emitAddTrajectory(topLeftQuarterCenter, id);
	}
}

void TrackedComponentView::swapIds()
{
	QList<QGraphicsItem *> allSelectedItems = this->scene()->selectedItems();
	if(allSelectedItems.size() != 2){
		qWarning() << "There need to be exactly two tracks selected to swap ID's!";
		return;
	}
	ComponentShape* shape0 = dynamic_cast<ComponentShape*>(allSelectedItems[0]);
	ComponentShape* shape1 = dynamic_cast<ComponentShape*>(allSelectedItems[1]);
	if(shape0 && shape1 && shape0->isSwappable() && shape1->isSwappable()){
		IModelTrackedTrajectory* trajectory0 = dynamic_cast<IModelTrackedTrajectory*>(shape0->getTrajectory());
		IModelTrackedTrajectory* trajectory1 = dynamic_cast<IModelTrackedTrajectory*>(shape1->getTrajectory());
		emitSwapIds(trajectory0, trajectory1);
	}
	else{
		qWarning() << "Selected objects are not swappable";
	}
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

void TrackedComponentView::connectShape(ComponentShape* shape) {
	CoreParameter* coreParams = dynamic_cast<CoreParameter*>(dynamic_cast<ControllerTrackedComponentCore*>(getController())->getCoreParameter());

	//set type
	//TODO bad code clean up
	IModelTrackedTrajectory* trajectory = shape->getTrajectory();

	IModelTrackedPoint* point = dynamic_cast<IModelTrackedPoint*>(trajectory->getChild(m_currentFrameNumber));
	if (point) { shape->setData(1, "point"); }
	IModelTrackedEllipse* ellipse = dynamic_cast<IModelTrackedEllipse*>(trajectory->getChild(m_currentFrameNumber));
	if (ellipse) { shape->setData(1, "ellipse"); }
	IModelTrackedRectangle* rectangle = dynamic_cast<IModelTrackedRectangle*>(trajectory->getChild(m_currentFrameNumber));
	if (rectangle) { shape->setData(1, "rectangle"); }
	IModelTrackedPolygon* polygon = dynamic_cast<IModelTrackedPolygon*>(trajectory->getChild(m_currentFrameNumber));
	if (polygon) { shape->setData(1, "polygon"); }


	//connect slots/signals
	QObject::connect(shape, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)), Qt::DirectConnection);
	QObject::connect(shape, SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*, uint)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveRemoveTrackEntity(IModelTrackedTrajectory*, uint)), Qt::DirectConnection);
	QObject::connect(shape, SIGNAL(emitMoveElement(IModelTrackedTrajectory*,QPoint, QPoint, uint, int)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveMoveElement(IModelTrackedTrajectory*, QPoint, QPoint, uint, int)), Qt::DirectConnection);
	QObject::connect(shape, SIGNAL(emitToggleFixTrack(IModelTrackedTrajectory*, bool)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SLOT(receiveToggleFixTrack(IModelTrackedTrajectory*,bool)), Qt::DirectConnection);
	QObject::connect(shape, SIGNAL(emitEntityRotation(IModelTrackedTrajectory*, double, double, uint)), dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), SIGNAL(emitEntityRotation(IModelTrackedTrajectory*, double, double, uint)), Qt::DirectConnection);
    QObject::connect(shape, &ComponentShape::emitGoToFrame, dynamic_cast<ControllerTrackedComponentCore*>(this->getController()), &ControllerTrackedComponentCore::emitGoToFrame, Qt::DirectConnection);


	QObject::connect(shape, SIGNAL(broadcastMove()), this, SLOT(receiveBroadcastMove()), Qt::DirectConnection);

	//set the shapes members
	shape->m_currentFramenumber = m_currentFrameNumber;
	shape->setMembers(coreParams);

	//set permissions
	shape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTMOVE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTMOVE]));
	shape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTREMOVE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTREMOVE]));
	shape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTSWAP, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTSWAP]));
	shape->setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>(ENUMS::COREPERMISSIONS::COMPONENTROTATE, m_permissions[ENUMS::COREPERMISSIONS::COMPONENTROTATE]));

	//update the shape
	shape->updateAttributes(m_currentFrameNumber);
}

bool TrackedComponentView::checkTrajectory(IModelTrackedTrajectory* trajectory){
	bool isVisualized = false;

	for (int i = 0; i < this->childItems().size(); i++) {
		ComponentShape* shape = dynamic_cast<ComponentShape*>(this->childItems()[i]);
		if (shape) {
			if(shape->getTrajectory() == trajectory){
				isVisualized = true;
				break;
			}
		}
	}
	return isVisualized;
}

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

void TrackedComponentView::receiveTracerFrameNumber(bool toggle) {
	QList<QGraphicsItem*> childrenItems = this->childItems();
	QGraphicsItem* childItem;
	foreach(childItem, childrenItems) {
		ComponentShape* childShape = dynamic_cast<ComponentShape*>(childItem);
		if (childShape) {
			childShape->receiveTracerFrameNumber(toggle);
		}
	}
}

void TrackedComponentView::receiveToggleAntialiasingEntities(bool toggle)
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
