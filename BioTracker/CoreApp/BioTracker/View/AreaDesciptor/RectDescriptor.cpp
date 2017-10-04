#include "RectDescriptor.h"

#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>

#include "util/misc.h"
#include "Model/AreaDescriptor/AreaInfoElement.h"
#include "Model/AreaDescriptor/AreaInfo.h"
#include <QGraphicsRectItem>

RectDescriptor::RectDescriptor(IController *controller, IModel *model) :
	AreaDescriptor(controller, model)
{
	setAcceptHoverEvents(true);
	//setAcceptedMouseButtons(Qt::MouseButtons::enum_type::LeftButton);

	_brush = QBrush(Qt::blue);

	//_isEllipse = true;
	_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
	for (int i = 0; i < 4; i++) {
		std::shared_ptr<QGraphicsRectItem> ri = std::make_shared<QGraphicsRectItem>(QRect(_v[i].x - 10, _v[i].y - 10, 20, 20), this);
		ri->setBrush(_brush);
		_rectification.push_back(ri);
	}

	for (int i = 0; i < 4; i++) {

		auto fst = _rectification[i];
		auto snd = _rectification[(i + 1) % 4];
		
		std::shared_ptr<QGraphicsLineItem> ri = std::make_shared<QGraphicsLineItem>(
			QLine(fst->rect().x() + 10, fst->rect().y() + 10, snd->rect().x() + 10, snd->rect().y() + 10), this);
		_rectificationLines.push_back(std::shared_ptr<QGraphicsLineItem>(ri));
	}

	//_ellipseRotation = 0.0;
	_isInit = false;
}

void RectDescriptor::init() {
	for (int i = 0; i < 4; i++) {
		_rectification[i]->setAcceptHoverEvents(true);
		_rectification[i]->installSceneEventFilter(this);
	}
}

void RectDescriptor::setBrush(QBrush brush) {
	for (int i = 0; i < 4; i++) {
		_rectification[i]->setBrush(brush);
	}
	_brush = brush;
}

bool RectDescriptor::inShape(QPoint p) {
	return false;
}

void RectDescriptor::updateRect() {
	setRect(getRect());
}

void RectDescriptor::setRect(std::vector<cv::Point> rect) {
	std::vector<std::shared_ptr<QGraphicsRectItem>> rectification;
	std::vector<std::shared_ptr<QGraphicsLineItem>> rectificationLines;

	_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();

	for (int i = 0; i < 4; i++) {
		std::shared_ptr<QGraphicsRectItem> ri = std::make_shared<QGraphicsRectItem>(QRect(rect[i].x - 10, rect[i].y - 10, 20, 20), this);
		ri->setBrush(_brush);
		rectification.push_back(ri);
	}

	for (int i = 0; i < 4; i++) {

		auto fst = rectification[i];
		auto snd = rectification[(i + 1) % 4];

		std::shared_ptr<QGraphicsLineItem> ri = std::make_shared<QGraphicsLineItem>(
			QLine(fst->rect().x() + 10, fst->rect().y() + 10, snd->rect().x() + 10, snd->rect().y() + 10), this);
		rectificationLines.push_back(std::shared_ptr<QGraphicsLineItem>(ri));
	}
	_rectification = rectification;
	_rectificationLines = rectificationLines;
}

std::vector<cv::Point> RectDescriptor::getRect() {
	return (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
}

RectDescriptor::~RectDescriptor()
{
}

void RectDescriptor::getNotified() {

}

QRectF RectDescriptor::boundingRect() const
{
	return QRect(10,10,10,10);
}

void RectDescriptor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!_isInit)
		init();
}

void RectDescriptor::updateLinePositions() {

	for (int i = 0; i < 4; i++) {

		auto fst = _rectification[i];
		auto snd = _rectification[(i + 1) % 4];
		auto ln = _rectificationLines[i];
		//float f1 = fst->rect().x() + 5, f2 = fst->rect().y() + 5, f3 = snd->rect().x() + 5, f4 = snd->rect().y() + 5;

		ln->setLine(QLine(fst->rect().x() + 10, fst->rect().y() + 10, snd->rect().x() + 10, snd->rect().y() + 10));
	}

}

bool RectDescriptor::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {

	return 0;
}
