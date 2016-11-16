#include "TrackedElement.h"
#include "QDebug"
#include "QRect"
#include "QBrush"
#include "QPainter"

TrackedElement::TrackedElement(QObject *parent, QString name) :
    ITrackedComponent(parent),
    name(name)
{
    x = 0;
    y = 0;
    pressed = false;
}

QString TrackedElement::getName()
{
    return name;
}

void TrackedElement::setX(int val)
{
    x = val;
}

void TrackedElement::setY(int val)
{
    y = val;
}

int TrackedElement::getX()
{
    return x;
}

int TrackedElement::getY()
{
    return y;
}

void TrackedElement::operate()
{
    qDebug() << "I am TrackedElement " <<  name;
}

QRectF TrackedElement::boundingRect() const
{
    return QRectF(0,0,100,100);
}

void TrackedElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush (Qt::blue);

    if(pressed)
    {
        brush.setColor(Qt::red);
    }
    else
    {
        brush.setColor(Qt::green);
    }

    painter->fillRect(rec,brush);
    painter->drawRect(rec);
}

void TrackedElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void TrackedElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}
