#include "EllipseDescriptor.h"
#include "Model/AreaDescriptor/AreaInfoElement.h"
#include "Utility/misc.h"

#include "QBrush"
#include "QPainter"
#include <QGraphicsSceneHoverEvent>
#include <qpixmap.h>

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
        bool inv = isInverted(fst->rect().x(), fst->rect().y(), _drag.x(), _drag.y());
        int x = fst->rect().x() + (_drag.x() - fst->rect().x()) / 2.0;
        int y = fst->rect().y() + (_drag.y() - fst->rect().y()) / 2.0;


        if (inv) {
            //Create a canvas to draw on
            QImage image(_vdimX, _vdimY, QImage::Format_ARGB32_Premultiplied);
            image.fill(Qt::transparent);
            QPainter p(&image);

            //Create dark rect which spans the screen
            p.setCompositionMode(QPainter::CompositionMode_SourceOver);
            QRectF r(0,0, _vdimX, _vdimY);
            QColor b;
            b.setRgb(0,0,0, 200);
            QBrush brush(b);
            p.fillRect(r, brush);
            p.drawRect(r);

            //Cut out the rectangle
            p.setCompositionMode(QPainter::CompositionMode_DestinationOut);
            b.setNamedColor("white");
            b.setAlpha(125);
            QBrush bbb(b);
            p.setBrush(bbb);
            p.drawEllipse(QRect(fst->rect().x(), fst->rect().y(), _drag.x() - fst->rect().x(), _drag.y() - fst->rect().y()));
            
            //Draw canvas onto the actual image
            painter->setOpacity(0.5); //mixed with the previous alphas!
            painter->drawImage(0,0, image);
        }
        else 
        {
            //This part is way simpler, as we only need to draw a somewhat transparent & filled ellipse
            QColor b;
            b.setNamedColor("black");
            b.setAlpha(125);
            QBrush brush(b);
            painter->setBrush(brush);
            painter->setOpacity(0.5);
            painter->drawEllipse(QRect(fst->rect().x(), fst->rect().y(), _drag.x() - fst->rect().x(), _drag.y() - fst->rect().y()));
        }
    }
}

bool EllipseDescriptor::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
	return 1;
}
