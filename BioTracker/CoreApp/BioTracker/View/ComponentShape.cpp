#include "ComponentShape.h"
#include "QBrush"
#include "QPainter"
#include "QMenu"
#include "QAction"
#include "QGraphicsScene"
#include "QGraphicsSceneContextMenuEvent"
#include "QDebug"
#include "QColorDialog"
#include "qwidgetaction.h"
#include "qlabel.h"
#include "assert.h"
#include "QTime"
#include "qlistwidget.h"
#include "QGraphicsProxyWidget"
#include "QVBoxLayout"

ComponentShape::ComponentShape(QGraphicsObject* parent, IModelTrackedTrajectory* trajectory, int id):
	QGraphicsObject(parent), m_trajectory(trajectory), m_id(id), m_parent(parent)
{
	setData(0, m_id);

	m_polygons = QList<QPolygonF>();
	m_useDefaultDimensions = true;
	m_penWidth = 0;
	m_penStyle = Qt::SolidLine;
	m_penStylePrev = Qt::SolidLine;
	m_marked = false;
	m_pMovable = true;
	m_pRemovable = true;
	m_pSwappable = true;
	m_fixed = false;
	m_currentFramenumber = 0;
	m_rotation = 0;
	m_trajectoryWasActiveOnce = false;

	m_tracingLayer = new QGraphicsRectItem();
	this->scene()->addItem(m_tracingLayer);

	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);

	setAcceptedMouseButtons(Qt::LeftButton);

	setVisible(true);
}

ComponentShape::~ComponentShape() {
    delete m_tracingLayer;
}

QRectF ComponentShape::boundingRect() const
{
	if (this->data(1) == "ellipse" || this->data(1) == "point" || this->data(1) == "rectangle") {
		return QRectF(0, 0, m_w, m_h);
	}
	else if (this->data(1) == "polygon") {
		//outer polygon bounding rect
		return m_polygons[0].boundingRect();
	}
	else {
		qDebug() << "Could not create a bounding rect for current track" << m_id;
		assert(0);
	}

}

QPainterPath ComponentShape::shape() const
{
	QPainterPath path;
	if (this->data(1) == "ellipse" || this->data(1) == "point") {
		path.addEllipse(0, 0, m_w, m_h);
	}
	else if (this->data(1) == "recatangle") {
		path.addRect(0, 0, m_w, m_h);
	}
	else if (this->data(1) == "polygon") {
		//outer polygon
		path.addPolygon(m_polygons[0]);
	}
	else {
		qDebug() << "Could not create a shape (interaction area) for current track" << m_id;
		assert(0);
	}
	return path;
}

void ComponentShape::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

   
    if (m_currentFramenumber < 0)
        return;
	//Antialiasing
	if (m_antialiasing) {
		painter->setRenderHint(QPainter::Antialiasing);
	}

	//check if scene is set
	if (!(this->scene())) {
		qDebug() << "componentscene is null\n";
	}

	QPen pen = QPen(m_penColor, m_penWidth, m_penStyle);
	QBrush brush = QBrush(m_brushColor);
	painter->setPen(pen);
	painter->setBrush(brush);

	// draw angleLine
	if (m_orientationLine) {
		QLineF angleline;
		angleline.setP1(QPointF(m_w / 2, m_h / 2));
		if (m_h > m_w) {
			angleline.setAngle(-90);
		}
		else {
			angleline.setAngle(0);
		}
		//TODO length changable
		angleline.setLength(100);
		painter->drawLine(angleline);
	}

	//TODO enums for types?

	//draw ellipse
	if (this->data(1) == "ellipse") {
		QRectF ellipse = QRectF(0, 0, m_w, m_h);
		painter->drawEllipse(ellipse);
	}

	//draw rectangle
	if (this->data(1) == "rectangle") {
		QRect rectangle = QRect(0, 0, m_w, m_h);
		painter->drawRect(rectangle);
	}

	//draw point
	//TODO how to visualize this the best way
	//a point should always have the same height and width
	if (this->data(1) == "point") {
		QRectF ellipse = QRectF(0, 0, m_w, m_h);
		painter->drawEllipse(ellipse);
	}

	//draw polygon
	if (this->data(1) == "polygon") {
		foreach(QPolygonF polygonF, m_polygons) {
			painter->drawPolygon(polygonF);
		}
	}

	// draw id in center

	painter->drawText(this->boundingRect(), Qt::AlignCenter, QString::number(m_id));

}

bool ComponentShape::advance()
{
	return false;
}

//put this in updateAttributes
bool ComponentShape::updatePosition(uint framenumber)
{
	//printf("update a shape\n");
	m_currentFramenumber = framenumber;

	if (m_trajectory && m_trajectory->size() != 0 && m_trajectory->getValid() && m_trajectory->getChild(framenumber)) {



		//update width and height for the latest tracked component
		updateAttributes(); 

		//for each point-like (point, rectangle, ellipse)
		IModelTrackedPoint* pointLike = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(framenumber));
		if (pointLike) {
			// if component and traj valid or current frame is 0 (video initialized) -> show and update
			//TODO undo/redo remove entity does not work on frame 0 because of m_currentFramenumber == 0!!!!!!!
			if ((pointLike->getValid() && m_trajectory->getValid()) || m_currentFramenumber == 0) {
				this->setPos(pointLike->getXpx() - m_w/2, pointLike->getYpx() - m_h/2);
				m_oldPos = this->pos().toPoint();
				QPointF point = this->pos();
				//create tracers
				trace();

				this->show();
				update();

				//qDebug() << "shape is updated to:" << pos();

				//printf("shape updated\n");
			}
			// else hide shape
			else {
				this->hide();
				update();
			}
			return true;
		}
		else {
			qDebug() << "no tracked component in this trajectory in frame: " << framenumber;
			return true;
		}
		//for each polygon
		IModelTrackedPolygon* polygons = dynamic_cast<IModelTrackedPolygon*>(m_trajectory->getChild(framenumber));
		if (polygons) {

		}
	}
	else {
		//printf("trajectory empty, delete child%i...\n", m_id);
		this->hide();
		m_tracingLayer->hide();
		delete this;
		return false;
	}

}
//enables possibility for plugin to change width, height, rotation
//TODO for polygons
void ComponentShape::updateAttributes()
{
	m_id = m_trajectory->getId();


	//update m_fixed
	m_fixed = m_trajectory->getFixed();

	if (!m_fixed) {m_penStyle = Qt::SolidLine;}
	else {m_penStyle = Qt::DotLine;}

	prepareGeometryChange();
	IModelTrackedPoint* pointLike = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(m_currentFramenumber));
	if (pointLike) {

		//update width and height or use defaults
		if (m_useDefaultDimensions) {
			if (pointLike->hasW()) {
				m_w = pointLike->getW();
			}
			else { m_w = m_wDefault; }

			if (pointLike->hasH()) {
				m_h = pointLike->getH();
			}
			else { m_h = m_hDefault; }
		}

		//update rotation
		this->setTransformOriginPoint(m_w / 2, m_h / 2);
		if (m_h > m_w) {
			m_rotation = -90 - pointLike->getDeg();
			this->setRotation(m_rotation);
		}
		else {
			m_rotation = -pointLike->getDeg();
			this->setRotation(m_rotation);
		}
	}
}

void ComponentShape::trace()
{
	//TRACING

	IModelTrackedPoint* currentChild = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(m_currentFramenumber));
	//return if current entity is not existant
	if (!currentChild)
		return;

	QPointF currentPoint = QPointF(currentChild->getXpx(), currentChild->getYpx());

	//update the tracing layer
	m_tracingLayer->setPos(this->pos());
	m_tracingLayer->setFlag(QGraphicsItem::ItemHasNoContents);
	m_tracingLayer->show();

	// really unefficient to flush each time
	//flush tracing shape history, open up the memory
    while (m_tracingLayer->childItems().size() > 0) {
		delete m_tracingLayer->childItems()[0];
    }

	////check if number of tracing children in tracing layer is correct
	////delete/add the difference
	//while (m_tracingLayer->childItems().size() > (int)floor(m_tracingLength / m_tracingSteps)) {
	//	delete m_tracingLayer->childItems()[0];
	//}
	//while (m_tracingLayer->childItems().size() < (int)floor(m_tracingLength / m_tracingSteps)) {
	//	m_tracingLayer
	//}


	if (m_trajectory->size() == 0 || m_tracingLength <= 0 || m_tracingStyle == "None") {
		return;
	}

	QPointF lastPointDifference = QPointF(0, 0) + QPointF(m_h / 2, m_w / 2);

	for (int i = 1; i <= m_tracingLength && i <= m_currentFramenumber; i += m_tracingSteps) {
			
		IModelTrackedPoint* historyChild = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(m_currentFramenumber - i));
		if (historyChild && historyChild->getValid()) {

			//positioning
			QPointF historyPoint = QPointF(historyChild->getXpx(), historyChild->getYpx());
			QPointF historyPointDifference = historyPoint - currentPoint;
			QPointF adjustedHistoryPointDifference = historyPointDifference + QPointF(m_w / 2, m_h / 2);

			//time degradation colors
			QPen timeDegradationPen = QPen(m_penColor, m_penWidth, m_penStyle);
			QBrush timeDegradationBrush = QBrush(m_brushColor);
			QColor timeDegradationBrushColor;
			QColor timeDegradationPenColor;

			if (m_tracingTimeDegradation == "Transparency") {
				timeDegradationPenColor = QColor(m_penColor.red(), m_penColor.green(), m_penColor.blue(), (200.0f - (200.0f / (float)m_tracingLength) * i) + 30);
				timeDegradationPen = QPen(timeDegradationPenColor, m_penWidth, Qt::SolidLine);

				timeDegradationBrushColor = QColor(m_brushColor.red(), m_brushColor.green(), m_brushColor.blue(), (200.0f - (200.0f / (float)m_tracingLength) * i) + 30);
				timeDegradationBrush = QBrush(timeDegradationBrushColor);

			}
			else if (m_tracingTimeDegradation == "False color") {
				float hue = (240.0f - ((240.0f / (float)m_tracingLength) * i));
				timeDegradationPenColor = QColor::fromHsv((int)hue, 255.0f, 255.0f);
				timeDegradationBrushColor = QColor::fromHsv((int)hue, 255.0f, 255.0f);
				timeDegradationPen = QPen(m_penColor, m_penWidth, m_penStyle);
				timeDegradationBrush = QBrush(timeDegradationBrushColor);
			}

			if (m_tracingStyle == "Shape") {


				//orientation line
				if (m_tracingOrientationLine) {
					QLineF orientationLine = QLineF();
					orientationLine.setP1(adjustedHistoryPointDifference);
					orientationLine.setAngle(historyChild->getDeg());
					orientationLine.setLength(15);

					QGraphicsLineItem* orientationItem = new QGraphicsLineItem(orientationLine, m_tracingLayer);
				}

				createShapeTracer(this->data(1), historyChild, adjustedHistoryPointDifference, timeDegradationPen, timeDegradationBrush);

			}

			//PATH
			else if (m_tracingStyle == "Path") {

				QLineF base = QLineF(lastPointDifference, adjustedHistoryPointDifference);
				QGraphicsLineItem* lineItem = new QGraphicsLineItem(base, m_tracingLayer);
				lineItem->setPen(QPen(timeDegradationBrushColor, m_penWidth, m_penStyle));

				lastPointDifference = adjustedHistoryPointDifference;
			}
			//ARROWPATH
			else if (m_tracingStyle == "ArrowPath") {
				QLineF base = QLineF(lastPointDifference, adjustedHistoryPointDifference);

				int armLength = std::floor(base.length() / 9) + 2;

				QLineF arm0 = base.normalVector();
				arm0.setLength(armLength);
				arm0.setAngle(base.angle() + 20);

				QLineF arm1 = base.normalVector();
				arm1.setLength(armLength);
				arm1.setAngle(base.angle() - 20);

				QGraphicsLineItem* baseItem = new QGraphicsLineItem(base, m_tracingLayer);
				baseItem->setPen(QPen(timeDegradationBrushColor, m_penWidth, m_penStyle));
				QGraphicsLineItem* arm0Item = new QGraphicsLineItem(arm0, m_tracingLayer);
				arm0Item->setPen(QPen(m_penColor, m_penWidth, m_penStyle));
				QGraphicsLineItem* arm1Item = new QGraphicsLineItem(arm1, m_tracingLayer);
				arm1Item->setPen(QPen(m_penColor, m_penWidth, m_penStyle));

				lastPointDifference = adjustedHistoryPointDifference;
			}

			//add framenumber to each tracer
			if (m_tracerFrameNumber) {
				uint tracerNumber = m_currentFramenumber - i;
				QFont font = QFont();
				int fontPixelSize = ((m_w + m_h) / 2) * m_tracerProportions * 0.5;
				font.setPixelSize(fontPixelSize);
				font.setItalic(true);
				font.setStyle(QFont::StyleItalic);
				QGraphicsSimpleTextItem* tracerNumberText = new QGraphicsSimpleTextItem(QString::number(tracerNumber), m_tracingLayer);
				tracerNumberText->setFont(font);
				tracerNumberText->setPos(adjustedHistoryPointDifference + QPointF(-m_w * m_tracerProportions / 3, -m_h * m_tracerProportions / 3));
			}
		}
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

	//qDebug() << "shape permission " << permission.first << "set to " << permission.second;
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

QPoint ComponentShape::getOldPos()
{
	return m_oldPos;
}

void ComponentShape::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	//qDebug() << "shape get dragged at:" << pos();
	m_mousePressTime = QTime::currentTime();
	m_mousePressTime.start();
	m_mousePressPos = pos().toPoint();
	//qDebug()<< "PRESS" << m_mousePressPos;

	if (event->button() == Qt::LeftButton) {
		// handle left mouse button here
		setCursor(Qt::ClosedHandCursor);
		//m_dragged = true;
		update();
	}
	//pass on
	QGraphicsItem::mousePressEvent(event);
}

void ComponentShape::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		setCursor(Qt::ArrowCursor);

		QPoint currentPos = this->pos().toPoint();
		int manhattanLength = (currentPos - m_mousePressPos).manhattanLength();
		int moveTime = m_mousePressTime.elapsed();
		if (manhattanLength > 5 || moveTime > 200) {
			m_dragged = true;
		}


		if (m_dragged) {

			//broadcast move so other selected elements get moved too
			// TODO? maybe unconventional and slow but couldn't find another way; Dropevents in view and dropevents in shape didn't seem to work
			broadcastMove();
		}
		else {
			this->setPos(m_mousePressPos);
			this->update();
		}
		m_dragged = false;
		//qDebug() << "DROP " << this->getId() << ": " << pos();

		update();
	}
	//pass on
	QGraphicsItem::mouseReleaseEvent(event);

}

void ComponentShape::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
	//pass on
	QGraphicsItem::mouseMoveEvent(event);
}

QVariant ComponentShape::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSelectedHasChanged && scene()) {
		if (this->isSelected()) {
			m_penColorLast = m_penColor;
			m_penColor = Qt::red;
			m_penStylePrev = m_penStyle;
			if (m_fixed) {
				m_penStyle = Qt::DashDotLine;
			}
			else {
				m_penStyle = Qt::DashLine;
			}
			this->setZValue(2);
			trace();
			update();
		}
		else {
			m_penColor = m_penColorLast;
			m_penStyle = m_penStylePrev;
			this->setZValue(0);
			trace();
			update();

		}
	}
	return QGraphicsItem::itemChange(change, value);
}

void ComponentShape::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QMenu menu;
	
	// create the info box
	QWidgetAction* infoBox = new QWidgetAction(this);

	QString info = QString("ID: ");
	info.append(QString::number(m_id));
	QLabel* infoLabel = new QLabel(info);
	infoLabel->setWordWrap(true);
	infoLabel->setStyleSheet("QLabel {font-weight: bold; text-align: center}");
	infoLabel->setAlignment(Qt::AlignCenter);
	infoBox->setDefaultWidget(infoLabel);
	menu.addAction(infoBox);
	menu.addSeparator();
	QAction *showInfoAction = menu.addAction("Show full info", dynamic_cast<ComponentShape*>(this), SLOT(createInfoWindow()));

	QAction *changeBrushColorAction = menu.addAction("Change fill color",dynamic_cast<ComponentShape*>(this),SLOT(changeBrushColor()));
	QAction *changePenColorAction = menu.addAction("Change border color", dynamic_cast<ComponentShape*>(this), SLOT(changePenColor()) );
	QAction *showProperties = menu.addSeparator();
	QAction *removeTrackAction = menu.addAction("Remove track", dynamic_cast<ComponentShape*>(this), SLOT(removeTrack()));
	QAction *removeTrackEntityAction = menu.addAction("Remove track entity", dynamic_cast<ComponentShape*>(this), SLOT(removeTrackEntity()));
	if (!m_pRemovable) { 
		removeTrackAction->setEnabled(false);
		removeTrackEntityAction->setEnabled(false);
	}

	QString fixText = m_fixed?"Unfix track":"Fix Track";
	QAction *fixTrackAction = menu.addAction(fixText, dynamic_cast<ComponentShape*>(this), SLOT(toggleFixTrack()));

	//TODO is ugly makenot ugly pls
	QString markText = m_marked?"Unmark":"Mark";
	QAction *markAction = menu.addAction(markText, dynamic_cast<ComponentShape*>(this), SLOT(markShape()));
	QAction *unmarkAction = menu.addAction(markText, dynamic_cast<ComponentShape*>(this), SLOT(unmarkShape()));
	if (m_marked) {
		markAction->setVisible(false);
		unmarkAction->setEnabled(true);
	}
	else {
		markAction->setEnabled(true);
		unmarkAction->setVisible(false);
	}
	
	QAction *selectedAction = menu.exec(event->screenPos());
}

void ComponentShape::createShapeTracer(QVariant type, IModelTrackedPoint * historyChild, QPointF pos, QPen pen, QBrush brush)
{
	if (this->data(1) == "point") {
		QGraphicsEllipseItem* tracePoint = new QGraphicsEllipseItem(m_tracingLayer);
		tracePoint->setPos(pos);
		tracePoint->setRect(QRect(-m_w * m_tracerProportions / 2, -m_h * m_tracerProportions / 2, m_w * m_tracerProportions, m_h * m_tracerProportions));
		//tracer orientation
		float tracerOrientation;
		if (m_h > m_w) { tracerOrientation = -90 - historyChild->getDeg(); }
		else { tracerOrientation = -historyChild->getDeg(); }
		tracePoint->setRotation(tracerOrientation);
		//set colors
		tracePoint->setPen(pen);
		tracePoint->setBrush(brush);

	}
	else if (this->data(1) == "ellipse") {
		QGraphicsEllipseItem* traceEllipse = new QGraphicsEllipseItem(m_tracingLayer);
		traceEllipse->setPos(pos);
		traceEllipse->setRect(QRect(-m_w * m_tracerProportions / 2, -m_h * m_tracerProportions / 2, m_w * m_tracerProportions, m_h * m_tracerProportions));
		//set orientation
		float tracerOrientation;
		if (m_h > m_w) { tracerOrientation = -90 - historyChild->getDeg(); }
		else { tracerOrientation = -historyChild->getDeg(); }
		traceEllipse->setRotation(tracerOrientation);
		//set colors
		traceEllipse->setPen(pen);
		traceEllipse->setBrush(brush);
	}
	else if (this->data(1) == "rectangle") {
		QGraphicsEllipseItem* traceRect = new QGraphicsEllipseItem(m_tracingLayer);
		traceRect->setPos(pos);
		traceRect->setRect(QRect(-m_w * m_tracerProportions / 2, -m_h * m_tracerProportions / 2, m_w * m_tracerProportions, m_h * m_tracerProportions));
		//set orientation
		float tracerOrientation;
		if (m_h > m_w) { tracerOrientation = -90 - historyChild->getDeg(); }
		else { tracerOrientation = -historyChild->getDeg(); }
		traceRect->setRotation(tracerOrientation);
		//set colors
		traceRect->setPen(pen);
		traceRect->setBrush(brush);
	}
	//TODO polygons
}

//SLOTS

void ComponentShape::changeBrushColor()
{
	this->m_brushColor = QColorDialog::getColor();
	trace();
	update();
}

void ComponentShape::changePenColor()
{
	QColor newColor = QColorDialog::getColor();

	if (!isSelected()) {
		m_penColorLast = m_penColor;
		m_penColor = newColor;

		trace();
		update();
	}
	else {
		m_penColorLast = newColor;
	}

}

void ComponentShape::changeBrushColor(QColor color)
{
	m_brushColor = color;

	trace();
	update();
}

void ComponentShape::changePenColor(QColor color)
{
	if (!isSelected()) {
		m_penColorLast = m_penColor;
		m_penColor = color;
		trace();
		update();
	}
	else {
		m_penColorLast = color;
	}
}

bool ComponentShape::removeTrack()
{
	if (m_pRemovable) {
		qDebug() << "Removing track...";

		//emit to set trajectory invalid 
		Q_EMIT emitRemoveTrajectory(m_trajectory);

	}
	else {
		qDebug() << "track is not removable";
	}
	return m_pRemovable;
}

bool ComponentShape::removeTrackEntity()
{
	if (m_pRemovable) {
		qDebug() << "Removing track entity...";

		//emit to set trajectory invalid 
		Q_EMIT emitRemoveTrackEntity(m_trajectory, m_currentFramenumber);
		//hide this shape
		//this->hide();
	}
	else {
		qDebug() << "track entity is not removable";
	}
	return m_pRemovable;
}

void ComponentShape::markShape(int penwidth)
{	
	if (!m_marked) {
		m_penWidth = 3;
	}
	else {
		m_penWidth = penwidth;
	}
	m_marked = true;
	trace();
	update();
}

void ComponentShape::unmarkShape()
{
	m_marked = false;
	m_penWidth = 0;

	trace();
	update();
}

void ComponentShape::toggleFixTrack()
{
	//if traj is currently fixed-> emit flase, else emit true
	bool fixToggle = m_fixed?false:true;
	Q_EMIT emitToggleFixTrack(m_trajectory, fixToggle);
	update();
	trace();
}

//TODO create ui file for this and do this there
void ComponentShape::createInfoWindow()
{
	//QGraphicsProxyWidget* infoWidgetProxy = new QGraphicsProxyWidget(this);
	QWidget* infoWidget = new QWidget();
	QVBoxLayout* vLayout = new QVBoxLayout();

	QHBoxLayout* hLayoutId = new QHBoxLayout();
	QLabel* idLabel = new QLabel("ID:");
	QLabel* id = new QLabel(QString::number(this->getId()));
	id->setTextInteractionFlags(Qt::TextSelectableByMouse);
	hLayoutId->addWidget(idLabel);
	hLayoutId->addWidget(id);


	//imodeltrackedtrajectory needs to output a imodelcomponenteuclidian2d object
	/*QHBoxLayout* hLayoutPos = new QHBoxLayout();
	QLabel* positionLabel = new QLabel("Position:");
	QLabel* position = new QLabel(QString::number(this->m_trajectory->getChild(m_currentFramenumber)->getX()) + QString::number(this->m_trajectory->getChild(m_currentFramenumber)->getY()));
	position->setTextInteractionFlags(Qt::TextSelectableByMouse);
	hLayoutId->addWidget(positionLabel);
	hLayoutId->addWidget(position);

	QHBoxLayout* hLayoutOrientation = new QHBoxLayout();
	QLabel* orientationLabel = new QLabel("Current orientation:");
	QLabel* orientation = new QLabel(QString::number(this->m_trajectory->getChild(m_currentFramenumber)->getDeg()));
	position->setTextInteractionFlags(Qt::TextSelectableByMouse);
	hLayoutId->addWidget(orientationLabel);
	hLayoutId->addWidget(orientation);*/

	QHBoxLayout* hLayoutSeen = new QHBoxLayout();
	QLabel* seenLabel = new QLabel("Seen for x frames:");
	QLabel* seen = new QLabel(QString::number(this->m_trajectory->validCount()));
	seen->setTextInteractionFlags(Qt::TextSelectableByMouse);
	hLayoutId->addWidget(seenLabel);
	hLayoutId->addWidget(seen);

	vLayout->addLayout(hLayoutId);
	//vLayout->addLayout(hLayoutPos);
	//vLayout->addLayout(hLayoutOrientation);
	vLayout->addLayout(hLayoutSeen);

	infoWidget->setLayout(vLayout);
	infoWidget->show();


}

void ComponentShape::receiveTracingLength(int tracingLength)
{
	m_tracingLength = tracingLength;
	trace();
	update();
}

void ComponentShape::receiveTracingStyle(QString style)
{
	m_tracingStyle = style;
	trace();
	update();
}

void ComponentShape::receiveTracingSteps(int steps)
{
	m_tracingSteps = steps;
	trace();
	update();
}

void ComponentShape::receiveTracingTimeDegradation(QString timeDegradation)
{
	m_tracingTimeDegradation = timeDegradation;
	trace();
	update();
}

void ComponentShape::receiveTracerProportions(float proportion)
{
	m_tracerProportions = proportion;
	trace();
	update();
}

void ComponentShape::receiveTracerOrientationLine(bool toggle)
{
	m_tracingOrientationLine = toggle;
	trace();
	update();
}

void ComponentShape::receiveTracerFrameNumber(bool toggle)
{
	m_tracerFrameNumber = toggle;
	trace();
	update();
}

void ComponentShape::receiveAntialiasing(bool toggle)
{
	m_antialiasing = toggle;
	trace();
	update();
}

void ComponentShape::receiveDimensions(int width, int height)
{
	m_useDefaultDimensions = false;
	m_w = width;
	m_h = height;
	updatePosition(m_currentFramenumber);
	trace();
	update();
}

void ComponentShape::setDimensionsToDefault()
{
	m_useDefaultDimensions = true;
	m_w = m_wDefault;
	m_h = m_hDefault;
	updatePosition(m_currentFramenumber);
	trace();
	update();
}

void ComponentShape::receiveToggleOrientationLine(bool toggle)
{
	m_orientationLine = toggle;
	trace();
	update();
}

void ComponentShape::receiveIgnoreZoom(bool toggle)
{
	setFlag(QGraphicsItem::ItemIgnoresTransformations, toggle);
	trace();
	update();
}
//set members from core params
void ComponentShape::setMembers(CoreParameter* coreParams)
{
	m_antialiasing = coreParams->m_antialiasingEntities;

	m_tracingStyle = coreParams->m_tracingStyle;
	m_tracingTimeDegradation = coreParams->m_tracingTimeDegradation;
	m_tracingLength = coreParams->m_tracingHistory;
	m_tracingSteps = coreParams->m_tracingSteps;
	m_tracerProportions = coreParams->m_tracerProportions;
	m_tracingOrientationLine = coreParams->m_tracerOrientationLine;
	m_tracerFrameNumber = coreParams->m_tracerFrameNumber;

	m_orientationLine = coreParams->m_trackOrientationLine;

	m_brushColor = *(coreParams->m_colorBrush);
	m_penColor = *(coreParams->m_colorBorder);

	m_dragged = false;
	setFlag(QGraphicsItem::ItemIgnoresTransformations, coreParams->m_ignoreZoom);

	// set dimensions and default dimensions
	if (coreParams->m_trackWidth > 0) {
		m_w = coreParams->m_trackWidth;
		m_wDefault = coreParams->m_trackWidth;
	}
	if (coreParams->m_trackHeight > 0) {
		m_h = coreParams->m_trackHeight;
		m_hDefault = coreParams->m_trackHeight;
	}

	update();
}
