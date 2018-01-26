#include "RectDescriptor.h"

#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>

#include "util/misc.h"
#include "Model/AreaDescriptor/AreaInfoElement.h"
#include "Model/AreaDescriptor/AreaInfo.h"
#include <QGraphicsRectItem>
#include "settings/Settings.h"

double orientation(cv::Point2f p1, cv::Point2f p2)
{
    cv::Point2f diff = p1 - p2;
    // need to check the origin of coorindiates
    return std::atan2(p1.y - p2.y, p1.x - p2.x);
}

RectDescriptor::RectDescriptor(IController *controller, IModel *model) :
	AreaDescriptor(controller, model)
{
    BioTracker::Core::Settings *settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
    _dragVectorId = -1;
    _dragType = BiotrackerTypes::AreaType::NONE;
	setAcceptHoverEvents(true);

	_brush = QBrush(Qt::blue);

	_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
    setRect(_v);

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
    std::vector<std::shared_ptr<QGraphicsSimpleTextItem>> rectificationNumbers;

    BioTracker::Core::Settings *settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);

	_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();

	for (int i = 0; i < 4; i++) {
		std::shared_ptr<QGraphicsRectItem> ri = std::make_shared<QGraphicsRectItem>(QRect(rect[i].x - 10, rect[i].y - 10, 20, 20), this);
		ri->setBrush(_brush);
		rectification.push_back(ri);
        //Numbers at corners
        if ((dynamic_cast<AreaInfoElement*>(getModel()))->getShowNumbers()) {
            std::shared_ptr<QGraphicsSimpleTextItem> ti = std::make_shared<QGraphicsSimpleTextItem>(QString::number(i), this);
            ti->setPos(_v[i].x + 10, _v[i].y + 10);
            ti->setFont(QFont("Arial", 20));
            rectificationNumbers.push_back(ti);
        }
	}

	for (int i = 0; i < 4; i++) {

		auto fst = rectification[i];
		auto snd = rectification[(i + 1) % 4];

        std::shared_ptr<QGraphicsLineItem> ri = std::make_shared<QGraphicsLineItem>(
            QLine(fst->rect().x() + 10, fst->rect().y() + 10, snd->rect().x() + 10, snd->rect().y() + 10), this);

        rectificationLines.push_back(ri);
        if ((dynamic_cast<AreaInfoElement*>(getModel()))->getShowNumbers()) {
            std::string label = "???";
            if (i % 2 == 0) {
                label = std::to_string(settings->getValueOrDefault<double>(AREADESCRIPTOR::RECT_H, 0));
                label.erase(label.find_last_not_of('0') + 1, std::string::npos);
            }
            else {
                label = std::to_string(settings->getValueOrDefault<double>(AREADESCRIPTOR::RECT_W, 0));
                label.erase(label.find_last_not_of('0') + 1, std::string::npos);
            }
            std::shared_ptr<QGraphicsSimpleTextItem> ti = std::make_shared<QGraphicsSimpleTextItem>(label.c_str(), this);
            cv::Point2f a(fst->rect().x() + 10, fst->rect().y() + 10);
            cv::Point2f b(snd->rect().x() + 10, snd->rect().y() + 10);
            double alpha = orientation(a, b) * 180 / CV_PI;
            //Fix upside down text
            alpha = alpha > 0 ? 180 - alpha : alpha;
            cv::Point2f c = a + (b - a) * 0.5;
            ti->setPos(c.x, c.y);
            ti->setFont(QFont("Arial", 20));
            ti->setRotation(-1*alpha);
            rectificationNumbers.push_back(ti);
        }
	}
	_rectification = rectification;
	_rectificationLines = rectificationLines;
    _rectificationNumbers = rectificationNumbers;
}

std::vector<cv::Point> RectDescriptor::getRect() {
	return (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
}

void RectDescriptor::receiveDragUpdate(BiotrackerTypes::AreaType vectorType, int id, double x, double y) {
    _dragType = (dynamic_cast<AreaInfoElement*>(getModel()))->getAreaType();
    if (_dragType == vectorType) {
        _dragVectorId = id;
        _drag = QPoint(x,y);
    }
    else {
        _dragVectorId = -1;
    }
    update();
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

    //TODO remove hardcoding and code duplication
    if (_dragVectorId >= 0 && _dragType != BiotrackerTypes::AreaType::NONE) {
        QColor transparentGray = Qt::gray;
        transparentGray.setAlphaF(0.75);
        painter->setPen(QPen(transparentGray, 1, Qt::SolidLine)); 
        painter->drawRect(_drag.x()-10, _drag.y()-10, 20, 20);
        int fstId = (_dragVectorId - 1) % 4;
        fstId = (fstId == -1 ? 3 : fstId);
        auto fst = _rectification[fstId];
        auto snd = _rectification[(_dragVectorId + 1) % 4];
        painter->drawLine(QLine(fst->rect().x() + 10, fst->rect().y() + 10, _drag.x(), _drag.y()));
        painter->drawLine(QLine(snd->rect().x() + 10, snd->rect().y() + 10, _drag.x(), _drag.y()));
    }
}

void RectDescriptor::updateLinePositions() {

	for (int i = 0; i < 4; i++) {

		auto fst = _rectification[i];
		auto snd = _rectification[(i + 1) % 4];
		auto ln = _rectificationLines[i];
		ln->setLine(QLine(fst->rect().x() + 10, fst->rect().y() + 10, snd->rect().x() + 10, snd->rect().y() + 10));
	}
    
}

bool RectDescriptor::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {

	return 0;
}
