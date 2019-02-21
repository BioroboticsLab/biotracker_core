#include "ComponentShape.h"

#include <cmath>
#include <cassert>

#include <QBrush>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QDebug>
#include <QColorDialog>
#include <QWidgetAction>
#include <QLabel>
#include <QTime>
#include <QListWidget>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QLineEdit>
#include <QAbstractSlider>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLinkedList>
#include <QPair>

#include "View/Utility/Tracer.h"


/*
*	This is the cpp file of the ComponentShape class
*	Collapse/Fold all methods to have a better overview.
*/

ComponentShape::ComponentShape(QGraphicsObject* parent,
	IModelTrackedTrajectory* trajectory, int id) :
	QGraphicsObject(parent), m_trajectory(trajectory), m_id(id), m_parent(parent)
{
	setData(0, m_id);
	setData(1, "point");

	m_polygons = QList<QPolygonF>();
	m_useDefaultDimensions = true;
	m_penWidth = 2;
	m_transparency = 255;
	m_penStyle = Qt::SolidLine;
	m_penStylePrev = Qt::SolidLine;
	m_marked = false;

	//set default permissions
	m_pMovable = true;
	m_pRemovable = true;
	m_pSwappable = true;
	m_pRotatable = true;

	m_fixed = false;
	m_currentFramenumber = 0;
	m_rotation = 0;
	//m_trajectoryWasActiveOnce = false;

	//create tracing layer
	m_tracingLayer = new QGraphicsRectItem();
	m_tracingLayer->setZValue(3);
	this->scene()->addItem(m_tracingLayer);

	//create orientation line
	m_rotationLine = QLineF();

	//set flags
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setAcceptedMouseButtons(Qt::LeftButton);

	setVisible(true);
}

ComponentShape::~ComponentShape() {
	delete m_tracingLayer;
}

//*****************************QGraphicsItem interface***************************************

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
		//Note: This log might not appear (on console) as the application dies of assert before the buffer is served
		qDebug() << "Could not create a bounding rect for current track " << m_id;
		assert(0);
	}

}

QPainterPath ComponentShape::shape() const
{
	QPainterPath path;
	if (this->data(1) == "ellipse") {
		path.addEllipse(0, 0, m_w, m_h);
	}
	else if (this->data(1) == "point") {
		int dim = m_w <= m_h ? m_w : m_h;
		// path.addEllipse(0, 0, dim , dim);
		QRectF ellipse;
		//ellipse = QRectF(0, m_h - m_w , m_w,  m_w);
		qreal origin_x = m_rotationLine.p1().x() - dim / 2;
		qreal origin_y = m_rotationLine.p1().y() - dim / 2;
		ellipse = QRectF(origin_x, origin_y, dim, dim);

		path.addEllipse(ellipse);

	}
	else if (this->data(1) == "rectangle") {
		path.addRect(0, 0, m_w, m_h);
	}
	else if (this->data(1) == "polygon") {
		//outer polygon
		path.addPolygon(m_polygons[0]);
	}
	else {
		qDebug() << "Could not create a interaction area for trajectory " << m_id;
		assert(0);
	}
	return path;
}

void ComponentShape::paint(QPainter * painter,
	const QStyleOptionGraphicsItem * option, QWidget * widget)
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

	// draw orientation line
	if (m_orientationLine && !m_rotationLine.isNull()) {
		painter->drawLine(m_rotationLine);
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
	//take the smaller of width and height
	//a point should always have the same height and width
	if (this->data(1) == "point") {
		QRectF ellipse;
		if (m_w <= m_h) {
			//ellipse = QRectF(0, m_h - m_w , m_w,  m_w);
			qreal origin_x = m_rotationLine.p1().x() - m_w / 2;
			qreal origin_y = m_rotationLine.p1().y() - m_w / 2;
			ellipse = QRectF(origin_x, origin_y, m_w, m_w);
		}
		else {
			qreal origin_x = m_rotationLine.p1().x() - m_h / 2;
			qreal origin_y = m_rotationLine.p1().y() - m_h / 2;
			ellipse = QRectF(origin_x, origin_y, m_h, m_h);
		}
		painter->drawEllipse(ellipse);
	}

	//draw polygon
	if (this->data(1) == "polygon") {
		foreach(QPolygonF polygonF, m_polygons) {
			painter->drawPolygon(polygonF);
		}
	}

	// draw id in center
	if (m_showId) {
		painter->drawText(this->boundingRect(), Qt::AlignCenter, QString::number(m_id));
	}
}

bool ComponentShape::advance()
{
	return false;
}

void ComponentShape::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	m_mousePressTime = QTime::currentTime();
	m_mousePressTime.start();
	m_mousePressPos = pos().toPoint();

	if (event->button() == Qt::LeftButton) {
		// handle left mouse button here
		setCursor(Qt::ClosedHandCursor);
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
			//maybe unconventional and slow but couldn't find another way; Dropevents in view and dropevents in shape didn't seem to work
			broadcastMove();
		}
		else {
			this->setPos(m_mousePressPos);
			this->update();
		}
		m_dragged = false;

		update();
	}
	//pass on
	QGraphicsItem::mouseReleaseEvent(event);

}

void ComponentShape::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
	//pass on
	QGraphicsItem::mouseMoveEvent(event);
}

QVariant ComponentShape::itemChange(GraphicsItemChange change,
	const QVariant &value)
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

	/*
	create the info box
	*/
	QWidgetAction* infoBox = new QWidgetAction(this);

	QString info = QString("ID: ");
	info.append(QString::number(m_id));
	QLabel* infoLabel = new QLabel(info);
	infoLabel->setWordWrap(true);
	infoLabel->setStyleSheet("QLabel {font-weight: bold; text-align: center}");
	infoLabel->setAlignment(Qt::AlignCenter);
	infoBox->setDefaultWidget(infoLabel);
	menu.addAction(infoBox);

	//
	menu.addSeparator();

	/*
	create set object name - line edit
	*/
	QWidgetAction* objectNameAction = new QWidgetAction(this);
	QLineEdit* objectEdit = new QLineEdit();

	if (objectName() == "") {
		objectEdit->setPlaceholderText("no object name set");
	}
	else {
		objectEdit->setText(objectName());
	}
	objectEdit->setAlignment(Qt::AlignHCenter);
	objectEdit->setFrame(false);
	objectEdit->setToolTip("Change the trajectory's name. This will not be saved in the data output (yet)!");

	QObject::connect(objectEdit, &QLineEdit::textEdited, this, &ComponentShape::setObjectNameContext);

	objectNameAction->setDefaultWidget(objectEdit);
	menu.addAction(objectNameAction);

	/*
	show info window for current frame
	*/
	menu.addSeparator();
	QAction *showInfoAction = menu.addAction("Show full info", dynamic_cast<ComponentShape*>(this), SLOT(createInfoWindow()));
	menu.addSeparator();

	/*
	coloring
	*/
	QAction *changeBrushColorAction = menu.addAction("Change fill color", dynamic_cast<ComponentShape*>(this), SLOT(changeBrushColor()));
	QAction *changePenColorAction = menu.addAction("Change border color", dynamic_cast<ComponentShape*>(this), SLOT(changePenColor()));

	/*
	transparency slider
	*/
	QMenu* transparencyMenu = new QMenu("Transparency");
	QWidgetAction* sliderBox = new QWidgetAction(this);
	QSlider* transparencySlider = new QSlider(Qt::Horizontal);

	transparencySlider->setMinimum(0);
	transparencySlider->setMaximum(255);
	transparencySlider->setSingleStep(1);
	transparencySlider->setTickPosition(QSlider::TicksBothSides);
	transparencySlider->setTickInterval(64);
	transparencySlider->setValue(m_transparency);

	QObject::connect(transparencySlider, &QSlider::sliderMoved, this, &ComponentShape::receiveTransparency);

	sliderBox->setDefaultWidget(transparencySlider);
	transparencyMenu->addAction(sliderBox);

	menu.addMenu(transparencyMenu);

	//
	menu.addSeparator();
	/*
	dimension menu
	*/
	QMenu* dimensionMenu = new QMenu("Dimensions");

	//Width
	QWidgetAction* widthBox = new QWidgetAction(this);
	QSpinBox* widthSpin = new QSpinBox;
	widthSpin->setPrefix("Width: ");
	widthSpin->setMinimum(1);
	widthSpin->setMaximum(100000);
	widthSpin->setValue(m_w);
	QObject::connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ComponentShape::receiveWidth);
	widthBox->setDefaultWidget(widthSpin);
	dimensionMenu->addAction(widthBox);

	//Height
	QWidgetAction* heightBox = new QWidgetAction(this);
	QSpinBox* heightSpin = new QSpinBox;
	heightSpin->setPrefix("Height: ");
	heightSpin->setMinimum(1);
	heightSpin->setMaximum(100000);
	heightSpin->setValue(m_h);
	QObject::connect(heightSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ComponentShape::receiveHeight);
	heightBox->setDefaultWidget(heightSpin);
	dimensionMenu->addAction(heightBox);

	//
	menu.addMenu(dimensionMenu);

	/*
	toggle orientation line
	*/

	QAction *showOrientationAction = menu.addAction("Show orientation line", dynamic_cast<ComponentShape*>(this), SLOT(receiveToggleOrientationLine(bool)));
	showOrientationAction->setCheckable(true);
	showOrientationAction->setChecked(m_orientationLine);

	/*
	toggle id
	*/

	QAction *showIDAction = menu.addAction("Show ID", dynamic_cast<ComponentShape*>(this), SLOT(receiveShowId(bool)));
	showIDAction->setCheckable(true);
	showIDAction->setChecked(m_showId);


	/*
	tracing menu
	*/
	QMenu* tracingMenu = new QMenu("Tracing");
	//tracing type
	QWidgetAction* typeBox = new QWidgetAction(this);
	QComboBox* typeCombo = new QComboBox;
	QStringList types;
	types << "No tracing" << "Shape" << "Path" << "Arrow Path";
	typeCombo->addItems(types);
	typeCombo->setCurrentText(m_tracingStyle);
	QObject::connect(typeCombo, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this, &ComponentShape::receiveTracingStyle);
	typeBox->setDefaultWidget(typeCombo);
	QMenu* tracingTypeMenu = new QMenu("Type");
	tracingTypeMenu->addAction(typeBox);
	tracingMenu->addMenu(tracingTypeMenu);

	//tracingHistory
	QWidgetAction* historyBox = new QWidgetAction(this);
	QSpinBox* historySpinBox = new QSpinBox;
	historySpinBox->setPrefix("History: ");
	historySpinBox->setMinimum(1);
	historySpinBox->setMaximum(100000);
	historySpinBox->setValue(m_tracingLength);
	QObject::connect(historySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ComponentShape::receiveTracingLength);
	historyBox->setDefaultWidget(historySpinBox);
	tracingMenu->addAction(historyBox);

	//tracingSteps
	QWidgetAction* stepsBox = new QWidgetAction(this);
	QSpinBox* stepsSpinBox = new QSpinBox;
	stepsSpinBox->setPrefix("Steps: ");
	stepsSpinBox->setMinimum(1);
	stepsSpinBox->setMaximum(100000);
	stepsSpinBox->setValue(m_tracingSteps);
	QObject::connect(stepsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ComponentShape::receiveTracingSteps);
	stepsBox->setDefaultWidget(stepsSpinBox);
	tracingMenu->addAction(stepsBox);

	//tracingDegradation
	QWidgetAction* degrBox = new QWidgetAction(this);
	QComboBox* degrCombo = new QComboBox;
	QStringList degrTypes;
	degrTypes << "None" << "Transparency" << "False color";
	degrCombo->addItems(degrTypes);
	degrCombo->setCurrentText(m_tracingTimeDegradation);
	QObject::connect(degrCombo, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this, &ComponentShape::receiveTracingTimeDegradation);
	degrBox->setDefaultWidget(degrCombo);
	QMenu* tracingTimeDegrMenu = new QMenu("Time degradation");
	tracingTimeDegrMenu->addAction(degrBox);
	tracingMenu->addMenu(tracingTimeDegrMenu);

	//toggle orientation line

	QAction *showTrOrientationAction = tracingMenu->addAction("Show tracer orientation line", dynamic_cast<ComponentShape*>(this), SLOT(receiveTracerOrientationLine(bool)));
	showTrOrientationAction->setCheckable(true);
	showTrOrientationAction->setChecked(m_tracingOrientationLine);

	//tracer number
	QAction *showTrNumberAction = tracingMenu->addAction("Show framenumber on tracers", dynamic_cast<ComponentShape*>(this), SLOT(receiveTracerFrameNumber(bool)));
	showTrNumberAction->setCheckable(true);
	showTrNumberAction->setChecked(m_tracerFrameNumber);

	//tracer proportions
	QWidgetAction* propBox = new QWidgetAction(this);
	QDoubleSpinBox* propSpinBox = new QDoubleSpinBox;
	propSpinBox->setPrefix("Proportions: ");
	propSpinBox->setDecimals(2);
	propSpinBox->setSingleStep(0.001f);
	propSpinBox->setMinimum(0.01f);
	propSpinBox->setMaximum(99.99f);
	propSpinBox->setValue(m_tracerProportions);
	QObject::connect(propSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ComponentShape::receiveTracerProportions);
	propBox->setDefaultWidget(propSpinBox);
	tracingMenu->addAction(propBox);


	menu.addMenu(tracingMenu);

	//
	menu.addSeparator();

	/*
	removing
	*/
	QAction *removeTrackAction = menu.addAction("Remove track", dynamic_cast<ComponentShape*>(this), SLOT(removeTrack()));
	QAction *removeTrackEntityAction = menu.addAction("Remove track entity", dynamic_cast<ComponentShape*>(this), SLOT(removeTrackEntity()));
	if (!m_pRemovable) {
		removeTrackAction->setEnabled(false);
		removeTrackEntityAction->setEnabled(false);
	}

	/*
	fixing
	*/
	QString fixText = m_fixed ? "Unfix track" : "Fix Track";
	QAction *fixTrackAction = menu.addAction(fixText, dynamic_cast<ComponentShape*>(this), SLOT(toggleFixTrack()));

	/*
	marking
	*/
	QString markText = m_marked ? "Unmark" : "Mark";
	QAction *markAction = menu.addAction(markText, dynamic_cast<ComponentShape*>(this), SLOT(markShape()));
	QAction *unmarkAction = menu.addAction(markText, dynamic_cast<ComponentShape*>(this), SLOT(unmarkShape()));
	markAction->setVisible(!m_marked);
	unmarkAction->setVisible(m_marked);

	//
	menu.addSeparator();

	/*
	morphing
	*/
	QMenu* morphMenu = new QMenu("Morph into...");
	if (data(1) == "rectangle" || data(1) == "ellipse" || data(1) == "point" || data(1) == "polygon") {
		QAction* morphPoint = morphMenu->addAction("Point", dynamic_cast<ComponentShape*>(this), SLOT(morphIntoPoint()));
		QAction* morphEllipse = morphMenu->addAction("Ellipse", dynamic_cast<ComponentShape*>(this), SLOT(morphIntoEllipse()));
		QAction* morphRect = morphMenu->addAction("Rectangle", dynamic_cast<ComponentShape*>(this), SLOT(morphIntoRect()));
		QAction* morphPoylgon = morphMenu->addAction("Polygon", dynamic_cast<ComponentShape*>(this), SLOT(morphIntoPolygon()));
	}
	else {
		morphMenu->setEnabled(false);
	}
	menu.addMenu(morphMenu);

	//
	QAction *selectedAction = menu.exec(event->screenPos());
}


//*************************************************************************

/// updates this with data from new current entity
bool ComponentShape::updateAttributes(uint frameNumber)
{
	m_currentFramenumber = frameNumber;

	// if trajectory does not exist anymore, delete the shape
	if (!m_trajectory) {
		this->hide();
		m_tracingLayer->hide();
		this->deleteLater();
		return false;
	}

	// check if trajectory is valid and current entity exists
	if (m_trajectory->size() != 0 && m_trajectory->getValid() && m_trajectory->getChild(frameNumber)) {
		m_id = m_trajectory->getId();
		//update m_fixed
		m_fixed = m_trajectory->getFixed();
		if (!m_fixed)
		{
			if (this->isSelected()) { m_penStyle = Qt::DashLine; }
			else { m_penStyle = Qt::SolidLine; }
		}
		else {
			if (this->isSelected()) { m_penStyle = Qt::DashDotLine; }
			else { m_penStyle = Qt::DotLine; }
		}

		//update dimensions
		prepareGeometryChange();

		//type checker
		bool hasType = false;

		//if point, ellipse, rectangle and valid
		IModelTrackedPoint* pointLike = dynamic_cast<IModelTrackedPoint*>(m_trajectory->getChild(m_currentFramenumber));
		if (pointLike && (pointLike->getValid())) {
			hasType = true;
			//update width and height or use defaults
			if (m_useDefaultDimensions) {
				if (pointLike->hasW()) { m_w = pointLike->getW(); }
				else { m_w = m_wDefault; }
				if (pointLike->hasH()) { m_h = pointLike->getH(); }
				else { m_h = m_hDefault; }
			}
			//update rotation
			if (pointLike->hasDeg()) {
				this->setTransformOriginPoint(m_w / 2, m_h / 2);
				if (m_h > m_w || data(1) == "polygon") {
					m_rotation = -90 - pointLike->getDeg();
					this->setRotation(m_rotation);
				}
				else {
					m_rotation = -pointLike->getDeg();
					this->setRotation(m_rotation);
				}

				//update rotation line
				m_rotationLine.setP1(QPointF(m_w / 2, m_h / 2));
				if (m_h > m_w || data(1) == "polygon") {
					m_rotationLine.setAngle(-90);
				}
				else {
					m_rotationLine.setAngle(0);
				}
				qreal length = (m_w + m_h) / 2 * 3;
				m_rotationLine.setLength(length);

				//update rotation handle
				m_rotationHandleLayer->setTransformOriginPoint(m_w / 2, m_h / 2);
				m_rotationHandleLayer->setRotation(0);
				m_rotationHandle->setPos(m_rotationLine.p2());
			}
			else {
				m_rotationLine = QLineF();
			}

			//update Position
			this->setPos(pointLike->getXpx() - m_w / 2, pointLike->getYpx() - m_h / 2);
			m_oldPos = this->pos().toPoint();


			/*
			-when morphing into polygon:
			-construct polygon from pos and width,height
			-pentagon

			  x / \
			   /   \
			   |   |
			   \_ _/

			*/
			m_polygons = QList<QPolygonF>();

			QPolygonF polygon;
			QPointF startPoint = QPointF(m_w / 2, 0);
			polygon << startPoint;
			polygon << QPointF(m_w, m_h / 2);
			polygon << QPointF(3 * m_w / 4, m_h);
			polygon << QPointF(m_w / 4, m_h);
			polygon << QPointF(0, m_h / 2);
			polygon << startPoint;

			if (polygon.isClosed()) {
				m_polygons.append(polygon);
			}

			//create tracers
			trace();

			this->show();
			update();

			return true;
		}
		else {
			this->hide();
			return true;
		}
		// if polygon
		IModelTrackedPolygon* polygons = dynamic_cast<IModelTrackedPolygon*>(m_trajectory->getChild(frameNumber));
		if (polygons && (polygons->getValid() || m_currentFramenumber == 0)) {
			hasType = true;
			//update polygon
			if (polygons->hasPolygon()) {
				m_polygons = polygons->getPolygon();
			}
			//also update position
			this->setPos(polygons->getXpx() - m_w / 2, polygons->getYpx() - m_h / 2);
			m_oldPos = this->pos().toPoint();

			//update width and height or use defaults --> for morphing
			if (m_useDefaultDimensions) {
				m_w = m_polygons[0].boundingRect().width();
				m_h = m_polygons[0].boundingRect().height();
			}

			//create tracers
			trace();

			this->show();
			update();

			return true;
		}
		//has no type
		if (!hasType) {
			qDebug() << "The current entity has no known type";

		}
	}
	else {
		//trajectory is empty or null or invald or current entity is null
		this->hide();
		m_tracingLayer->hide();
		//delete this;
		return false;
	}
}

/// attemps to draw tracers, if _tracingStyle is set tracers are drawn
void ComponentShape::trace()
{
	//TRACING

	IModelTrackedPoint* currentChild = dynamic_cast<IModelTrackedPoint*>
		(m_trajectory->getChild(m_currentFramenumber));
	//return if current entity is not existant
	if (!currentChild)
		return;

	QPointF currentPoint = QPointF(currentChild->getXpx(), currentChild->getYpx());

	//update the tracing layer
	m_tracingLayer->setPos(this->pos());
	m_tracingLayer->setFlag(QGraphicsItem::ItemHasNoContents);
	m_tracingLayer->show();

	// really unefficient to flush each time
	//flush tracing shape history, open up the memory --> thats slow
	QList<QGraphicsItem *>  tracers = m_tracingLayer->childItems();
	foreach(QGraphicsItem * tracer, tracers)
	{
		delete tracer;
	}

	//return if tracing disabled
	if (m_trajectory->size() == 0 || m_tracingLength <= 0 || m_tracingStyle == "No tracing") {
		return;
	}

	QPointF lastPointDifference = QPointF(0, 0);// + QPointF(m_h / 2, m_w / 2);

	//create each n'th (m_tracingSteps) tracer of tracing history (m_tracingLength)
	for (int i = 1; i <= m_tracingLength && i <= m_currentFramenumber; i += m_tracingSteps) {

		IModelTrackedPoint* historyChild = dynamic_cast<IModelTrackedPoint*>
			(m_trajectory->getChild(m_currentFramenumber - i));
		if (historyChild && historyChild->getValid()) {

			//positioning
			QPointF historyPoint = QPointF(historyChild->getXpx(), historyChild->getYpx());
			QPointF historyPointDifference = historyPoint - currentPoint;
			QPointF adjustedHistoryPointDifference = historyPointDifference;

			//time degradation colors
			QPen timeDegradationPen = QPen(m_penColor, m_penWidth, m_penStyle);
			QBrush timeDegradationBrush = QBrush(m_brushColor);
			QColor timeDegradationBrushColor;
			QColor timeDegradationPenColor;

			if (m_tracingTimeDegradation == "Transparency") {

				float tr = (float)m_transparency;
				float trForThis = tr != 0 ? tr - (tr / (float)m_tracingLength) * (i - 1) : 0.0f;

				timeDegradationPenColor = QColor(m_penColor.red(),
					m_penColor.green(), m_penColor.blue(), trForThis);
				timeDegradationPen = QPen(timeDegradationPenColor, m_penWidth, Qt::SolidLine);

				timeDegradationBrushColor = QColor(m_brushColor.red(),
					m_brushColor.green(), m_brushColor.blue(), trForThis);
				timeDegradationBrush = QBrush(timeDegradationBrushColor);

			}
			else if (m_tracingTimeDegradation == "False color") {
				float hue = (240.0f - ((240.0f / (float)m_tracingLength) * i));
				timeDegradationPenColor = QColor::fromHsv((int)hue, 255.0f, 255.0f);
				timeDegradationPenColor.setAlpha(m_transparency);
				timeDegradationBrushColor = QColor::fromHsv((int)hue, 255.0f, 255.0f);
				timeDegradationBrushColor.setAlpha(m_transparency);
				timeDegradationPen = QPen(m_penColor, m_penWidth, m_penStyle);
				timeDegradationBrush = QBrush(timeDegradationBrushColor);
			}

			// check tracing type

			//SHAPE
			if (m_tracingStyle == "Shape") {

				//orientation line
				if (m_tracingOrientationLine) {
					QLineF orientationLine = QLineF();
					orientationLine.setP1(adjustedHistoryPointDifference);
					orientationLine.setAngle(historyChild->getDeg());
					qreal length = (m_w + m_h) / 2 / m_tracerProportions * 3;
					orientationLine.setLength(15);

					QGraphicsLineItem* orientationItem = new QGraphicsLineItem(orientationLine,
						m_tracingLayer);
				}

				float tracerDeg = historyChild->hasDeg() ? historyChild->getDeg() : 0.0;
				float tracerW = m_w * m_tracerProportions;
				float tracerH = m_h * m_tracerProportions;
				int tracerNumber = m_currentFramenumber - i;

				Tracer* tracer = new Tracer(this->data(1), tracerNumber, tracerDeg,
					adjustedHistoryPointDifference, tracerW, tracerH,
					timeDegradationPen, timeDegradationBrush, m_tracingLayer);

				QObject::connect(tracer, &Tracer::emitGoToFrame,
					this, &ComponentShape::emitGoToFrame);
			}

			//PATH
			else if (m_tracingStyle == "Path") {

				if (lastPointDifference != adjustedHistoryPointDifference) {

					QLineF base = QLineF(lastPointDifference, adjustedHistoryPointDifference);
					QGraphicsLineItem* lineItem = new QGraphicsLineItem(base, m_tracingLayer);
					lineItem->setPen(QPen(timeDegradationPenColor, m_penWidth, m_penStyle));

					lastPointDifference = adjustedHistoryPointDifference;
				}
			}

			//ARROWPATH
			else if (m_tracingStyle == "Arrow path") {

				if (lastPointDifference != adjustedHistoryPointDifference) {

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
					arm0Item->setPen(timeDegradationPen);
					QGraphicsLineItem* arm1Item = new QGraphicsLineItem(arm1, m_tracingLayer);
					arm1Item->setPen(timeDegradationPen);

					lastPointDifference = adjustedHistoryPointDifference;

				}
			}

			//add framenumber to each tracer
			if (m_tracerFrameNumber) {
				uint tracerNumber = m_currentFramenumber - i;
				QFont font = QFont();
				int fontPixelSize = (int)((m_w + m_h) / 5) * m_tracerProportions;
				font.setPixelSize(fontPixelSize);
				//font.setBold(true);
				QGraphicsSimpleTextItem* tracerNumberText =
					new QGraphicsSimpleTextItem(QString::number(tracerNumber), m_tracingLayer);
				tracerNumberText->setFont(font);
				tracerNumberText->setBrush(QBrush(Qt::white)); //sloooow
				QPen pen = QPen(Qt::black);
				pen.setWidth(0);
				tracerNumberText->setPen(pen); //sloooow
				tracerNumberText->setPos(adjustedHistoryPointDifference +
					QPointF(-m_w * m_tracerProportions / 2.0f, -m_h * m_tracerProportions / 5));
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
		break;
	case ENUMS::COREPERMISSIONS::COMPONENTROTATE:
		m_pRotatable = permission.second;

		if (m_pRotatable) {
			m_rotationHandleLayer->setVisible(m_orientationLine);
			m_rotationHandle->setVisible(m_orientationLine);
		}
		else {
			m_rotationHandleLayer->hide();
			m_rotationHandle->hide();
		}
		break;
	}
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

bool ComponentShape::isRotatable()
{
	return m_pRotatable;
}

QPoint ComponentShape::getOldPos()
{
	return m_oldPos;
}

//set default members from core params
void ComponentShape::setMembers(CoreParameter* coreParams)
{
	//from coreParams
	m_antialiasing = coreParams->m_antialiasingEntities;


	m_tracingStyle = coreParams->m_tracingStyle;
	m_tracingTimeDegradation = coreParams->m_tracingTimeDegradation;
	m_tracingLength = coreParams->m_tracingHistory;
	m_tracingSteps = coreParams->m_tracingSteps;
	m_tracerProportions = coreParams->m_tracerProportions;
	m_tracingOrientationLine = coreParams->m_tracerOrientationLine;
	m_tracerFrameNumber = coreParams->m_tracerFrameNumber;

	m_orientationLine = coreParams->m_trackOrientationLine;
	m_showId = coreParams->m_trackShowId;

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

	//initialize helpers
	m_rotationHandleLayer = new QGraphicsRectItem(this);
	m_rotationHandleLayer->setPos(0, 0);
	m_rotationHandleLayer->setFlag(QGraphicsItem::ItemHasNoContents);

	m_rotationHandle = new RotationHandle(QPoint(m_w / 2, m_h / 2), m_rotationHandleLayer);
	m_rotationHandle->setAntialiasing(m_antialiasing);
	QObject::connect(m_rotationHandle, &RotationHandle::emitShapeRotation, this, &ComponentShape::receiveShapeRotation);

	if (m_pRotatable) {
		m_rotationHandleLayer->setVisible(m_orientationLine);
		m_rotationHandle->setVisible(m_orientationLine);
	}

	update();
}

double ComponentShape::constrainAngle(double x) {
	x = fmod(x, 360);
	if (x < 0)
		x += 360;
	return x;
}


//################################SLOTS#############################

//**************************context menu actions********************

void ComponentShape::changeBrushColor()
{

	this->m_brushColor = QColorDialog::getColor(m_brushColor, nullptr, QString("Choose fill color"), QColorDialog::ShowAlphaChannel);
	//m_transparency = m_brushColor.alpha();
	trace();
	update();
}

void ComponentShape::changePenColor()
{
	QColor newColor = QColorDialog::getColor(m_penColor, nullptr, QString("Choose border color"), QColorDialog::ShowAlphaChannel);
	//m_transparency = newColor.alpha();

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
		qDebug() << "Track is not removable";
	}
	return m_pRemovable;
}

bool ComponentShape::removeTrackEntity()
{
	if (m_pRemovable) {
		qDebug() << "Removing track entity...";

		//emit to set trajectory invalid 
		Q_EMIT emitRemoveTrackEntity(m_trajectory, m_currentFramenumber);
	}
	else {
		qDebug() << "Track entity is not removable";
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
	m_penWidth = 2;

	trace();
	update();
}

void ComponentShape::toggleFixTrack()
{
	//if traj is currently fixed-> emit flase, else emit true
	bool fixToggle = m_fixed ? false : true;
	Q_EMIT emitToggleFixTrack(m_trajectory, fixToggle);
	update();
	trace();
}

//TODO create ui file for this and do this there
void ComponentShape::createInfoWindow()
{
	QTableWidget* infoTable = new QTableWidget();

	infoTable->setRowCount(0);
	infoTable->setColumnCount(2);

	infoTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Property"));
	infoTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));
	infoTable->verticalHeader()->hide();

	QLinkedList<QPair<QString, QString>> infoList;
	infoList.append(QPair<QString, QString>("ID", QString::number(m_id)));
	infoList.append(QPair<QString, QString>("Framenumber", QString::number(m_currentFramenumber)));
	infoList.append(QPair<QString, QString>("Seen for frames", QString::number(m_trajectory->validCount())));
	infoList.append(QPair<QString, QString>("Width", QString::number(m_w)));
	infoList.append(QPair<QString, QString>("Height", QString::number(m_h)));
	infoList.append(QPair<QString, QString>("Orientation (in Degrees)", QString::number(m_rotation)));
	infoList.append(QPair<QString, QString>("Fixed", QString::number(m_fixed)));

	QLinkedList<QPair<QString, QString>>::const_iterator info;
	for (info = infoList.constBegin(); info != infoList.constEnd(); ++info) {
		infoTable->insertRow(infoTable->rowCount());


		QTableWidgetItem* infoKey = new QTableWidgetItem(info->first);
		QTableWidgetItem* infoKeyInfo = new QTableWidgetItem(info->second);
		infoKey->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		infoKeyInfo->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		infoTable->setItem(infoTable->rowCount() - 1, 0, infoKey);
		infoTable->setItem(infoTable->rowCount() - 1, 1, infoKeyInfo);

	}

	//infoTable->horizontalHeader()->setStretchLastSection( true ); 
	infoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	QWidget* infoWidget = new QWidget();
	const QString title = QString("Information for track %1 on frame %2").arg(QString::number(m_id), QString::number(m_currentFramenumber));
	infoWidget->setWindowTitle(title);
	QVBoxLayout* vLayout = new QVBoxLayout();


	infoWidget->resize(infoTable->size());
	vLayout->addWidget(infoTable);
	infoWidget->setLayout(vLayout);

	infoWidget->show();


}

// this is only visual, the object name is not set in the exported file 
void ComponentShape::setObjectNameContext(QString name) {
	setObjectName(name);
	m_trajectory->setObjectName(name);
}

void ComponentShape::morphIntoRect() {
	setData(1, "rectangle");
	updateAttributes(m_currentFramenumber);
	trace();
}
void ComponentShape::morphIntoEllipse() {
	setData(1, "ellipse");
	updateAttributes(m_currentFramenumber);
	trace();
}
void ComponentShape::morphIntoPoint() {
	setData(1, "point");
	updateAttributes(m_currentFramenumber);
	trace();
}
void ComponentShape::morphIntoPolygon()
{
	setData(1, "polygon");

	updateAttributes(m_currentFramenumber);
	trace();
}

//************************************************************************ 

//*************************** SLOTS to set members*****************************
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
	m_rotationHandle->setAntialiasing(toggle);
	trace();
	update();
}

void ComponentShape::receiveDimensions(int width, int height)
{
	m_useDefaultDimensions = false;
	m_w = width;
	m_h = height;
	updateAttributes(m_currentFramenumber);
	trace();
	update();
}

void ComponentShape::receiveHeight(int height) {
	m_useDefaultDimensions = false;
	m_h = height;
	updateAttributes(m_currentFramenumber);
	trace();
	update();
}

void ComponentShape::receiveWidth(int width) {
	m_useDefaultDimensions = false;
	m_w = width;
	updateAttributes(m_currentFramenumber);
	trace();
	update();
}

void ComponentShape::setDimensionsToDefault()
{
	m_useDefaultDimensions = true;
	m_w = m_wDefault;
	m_h = m_hDefault;
	updateAttributes(m_currentFramenumber);
	trace();
	update();
}

void ComponentShape::receiveToggleOrientationLine(bool toggle)
{
	m_orientationLine = toggle;
	if (m_pRotatable) {
		m_rotationHandleLayer->setVisible(toggle);
		m_rotationHandle->setVisible(toggle);
	}
	trace();
	update();
}

void ComponentShape::receiveShowId(bool toggle)
{
	m_showId = toggle;
	trace();
	update();
}

void ComponentShape::receiveShapeRotation(double angle, bool rotateEntity)
{
	m_rotationHandleLayer->setTransformOriginPoint(m_w / 2, m_h / 2);
	//m_rotationHandleLayer->setRotation(-angle);

	this->setTransformOriginPoint(m_w / 2, m_h / 2);
	if (m_h > m_w || data(1) == "polygon") {
		m_rotationHandleLayer->setRotation(-angle + 90);
		this->setRotation(m_rotation + angle - 90);
	}
	else {
		m_rotationHandleLayer->setRotation(-angle);
		this->setRotation(m_rotation + angle);
	}

	if (rotateEntity) {

		this->pos();

		double toAngle = -angle - m_rotation;
		double oldAngle = -m_rotation;

		double toAngleNorm = constrainAngle(toAngle);
		double oldAngleNorm = constrainAngle(oldAngle);
		Q_EMIT emitEntityRotation(m_trajectory, oldAngleNorm, toAngleNorm, m_currentFramenumber);
	}

	update();
}

//currently disabled
void ComponentShape::receiveIgnoreZoom(bool toggle)
{
	setFlag(QGraphicsItem::ItemIgnoresTransformations, toggle);
	trace();
	update();
}

void ComponentShape::receiveTransparency(int alpha) {
	m_transparency = alpha;
	m_brushColor.setAlpha(alpha);
	m_penColor.setAlpha(alpha);

	QColor penColorHandle = m_rotationHandle->pen().color();
	penColorHandle.setAlpha(alpha);
	m_rotationHandle->setPen(QPen(penColorHandle));

	QColor brushColorHandle = m_rotationHandle->brush().color();
	brushColorHandle.setAlpha(alpha);
	m_rotationHandle->setBrush(QBrush(brushColorHandle));

	trace();
	update();

}
