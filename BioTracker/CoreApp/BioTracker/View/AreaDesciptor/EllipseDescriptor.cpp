#include "EllipseDescriptor.h"

#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>

#include "Model/AreaDescriptor/AreaInfoElement.h"
#include "util/misc.h"

EllipseDescriptor::EllipseDescriptor(IController *controller, IModel *model) :
	AreaDescriptor(controller, model)
{
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::MouseButtons::enum_type::LeftButton);

	_brush = QBrush(Qt::blue);


	_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
	for (int i = 0; i < 4; i++) {
		std::shared_ptr<QGraphicsRectItem> ri = std::make_shared<QGraphicsRectItem>(QRect(_v[i].x - 10, _v[i].y - 10, 20, 20), this);
		ri->setBrush(_brush);
	}

	updateEllipse();

	_isInit = false;
}

void EllipseDescriptor::init() {
	_rectificationMarkerOrig->setBrush(_brush);
	_rectificationMarkerEnd->setBrush(_brush);
}

void EllipseDescriptor::updateEllipse() {

	_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
	_rectification = std::make_shared<QGraphicsEllipseItem>(QRect(_v[0].x, _v[0].y, _v[1].x-_v[0].x, _v[1].y- _v[0].y), this);
	_rectificationMarkerOrig = std::make_shared<QGraphicsRectItem>(QRect(_v[0].x, _v[0].y, 10, 10), this);
	_rectificationMarkerEnd = std::make_shared<QGraphicsRectItem>(QRect(_v[1].x, _v[1].y, 10, 10), this);
	_rectificationMarkerOrig->setBrush(_brush);
	_rectificationMarkerEnd->setBrush(_brush);
}

EllipseDescriptor::~EllipseDescriptor()
{
}

void EllipseDescriptor::setBrush(QBrush brush) {
	_brush = brush;
	_rectificationMarkerOrig->setBrush(_brush);
	_rectificationMarkerEnd->setBrush(_brush);
}

void EllipseDescriptor::updateRect() {
	setRect(getRect());
}

void EllipseDescriptor::setRect(std::vector<cv::Point> rect) {
	if(rect.size() >= 2){
		_v = (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
		updateEllipse();
	}
}

void EllipseDescriptor::receiveDragUpdate(BiotrackerTypes::AreaType vectorType, int id, double x, double y) {
    int atype = (dynamic_cast<AreaInfoElement*>(getModel()))->getAreaType();
    if (atype == vectorType) {
        _dragVectorId = id;
        _drag = QPoint(x, y);        
    }
    else {
        _dragVectorId = -1;
    }
    update();
}

std::vector<cv::Point> EllipseDescriptor::getRect() {
	return (dynamic_cast<AreaInfoElement*>(getModel()))->getVertices();
}


void EllipseDescriptor::getNotified() {

}

QRectF EllipseDescriptor::boundingRect() const
{
	return QRect(10, 10, 10, 10);
}

bool isInverted(int x1, int y1, int x2, int y2) {
    if ((x1 > x2 && y1 < y2) || (x1 < x2 && y1 > y2)) {
        return true;
    }
    return false;
}

#include <qstatictext.h>
#include <qtextformat.h>
void EllipseDescriptor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!_isInit)
		init();

    //TODO remove hardcoding and code duplication
    if (_dragVectorId >= 0) {
        QColor transparentGray = Qt::gray;
        transparentGray.setAlphaF(0.75);
        painter->setPen(QPen(transparentGray, 1, Qt::SolidLine));
        painter->drawRect(_drag.x(), _drag.y(), 10, 10);

        auto fst = _dragVectorId != 0 ? _rectificationMarkerOrig : _rectificationMarkerEnd;
        //auto snd = _dragVectorId == 0 ? _rectificationMarkerOrig : _rectificationMarkerEnd;
        bool inv = isInverted(fst->rect().x(), fst->rect().y(), _drag.x(), _drag.y());
        int x = fst->rect().x() + (_drag.x() - fst->rect().x()) / 2.0;
        int y = fst->rect().y() + (_drag.y() - fst->rect().y()) / 2.0;
        //QStaticText lOut;
        //lOut.setText("Outside");
        //QTextFormat fmt;
        //fmt.setProperty(QTextFormat::FontPointSize, 40);
        //lOut.setTextFormat(fmt);
        if (inv) {
            painter->drawText(x, y, 100, 40, Qt::AlignHCenter, "Outside");
            painter->drawText(0, 40, "inside");
        }
        else 
        {
            painter->drawText(0, 40, "Outside");
            painter->drawText(x, y, 100, 40, Qt::AlignHCenter, "inside");
        }
        painter->drawEllipse(QRect(fst->rect().x(), fst->rect().y(), _drag.x() - fst->rect().x(), _drag.y() - fst->rect().y()));
    }
}

bool EllipseDescriptor::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
	return 1;
}
