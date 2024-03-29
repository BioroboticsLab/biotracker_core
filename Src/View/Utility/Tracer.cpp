#include "Tracer.h"
#include "QDebug"
#include "QPen"
#include "QBrush"
#include "QPainter"
#include "QMenu"
#include "QGraphicsSceneContextMenuEvent"

Tracer::Tracer(QVariant                    type,
               int                         frame,
               float                       orientation,
               QPointF                     pos,
               float                       w,
               float                       h,
               QPen                        pen,
               QBrush                      brush,
               QAbstractGraphicsShapeItem* parent)
: QAbstractGraphicsShapeItem(parent)
, _type(type.toString())
, _frame(frame)
, _w(w)
, _h(h)
{
    setPos(pos);
    setPen(pen);
    setBrush(brush);
    setRotation(_h > _w ? -90 - orientation : -orientation);
}

Tracer::~Tracer()
{
}

QRectF Tracer::boundingRect() const
{
    return QRect(-_w / 2, -_h / 2, _w, _h);
}

void Tracer::paint(QPainter*                       painter,
                   const QStyleOptionGraphicsItem* option,
                   QWidget*                        widget)
{
    painter->setPen(pen());
    painter->setBrush(brush());

    if (_type == "point") {
        int dim = _w <= _h ? _w : _h;
        painter->drawEllipse(QRect(-dim / 2, -dim / 2, dim, dim));
    } else if (_type == "ellipse") {

        painter->drawEllipse(QRect(-_w / 2, -_h / 2, _w, _h));
    } else if (_type == "rectangle") {

        painter->drawRect(QRect(-_w / 2, -_h / 2, _w, _h));
    }
    // default for polygon shape are point tracers for simplicity
    else if (_type == "polygon") {
        int dim = _w <= _h ? _w : _h;
        painter->drawEllipse(QRect(-dim / 2, -dim / 2, dim, dim));
    }
}

void Tracer::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QMenu menu;
    menu.addAction("Go to frame ",
                   dynamic_cast<Tracer*>(this),
                   SLOT(goToFrame()));
    menu.exec(event->screenPos());
}

void Tracer::goToFrame()
{
    Q_EMIT emitGoToFrame(_frame);
}
