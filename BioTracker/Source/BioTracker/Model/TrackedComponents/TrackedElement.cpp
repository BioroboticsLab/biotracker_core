#include "TrackedElement.h"
#include "QDebug"
#include "QRect"
#include "QBrush"
#include "QPainter"

TrackedElement::TrackedElement(QObject *parent, QString name) :
    IModelTrackedComponent(parent),
    name(name)
{
    x = 100;
    y = 80;
    mPressed = false;
}

QString TrackedElement::getName()
{
    return name;
}

void TrackedElement::setX(int val)
{
    x = val;
    Q_EMIT notifyView();

}

void TrackedElement::setY(int val)
{
    y = val;
    Q_EMIT notifyView();

}

void TrackedElement::pressed()
{
    mPressed = true;
    Q_EMIT notifyView();

}

void TrackedElement::notPressed()
{
    mPressed = false;
    Q_EMIT notifyView();
}

int TrackedElement::getX()
{
    return x;
}

int TrackedElement::getY()
{
    return y;
}

bool TrackedElement::getPressedStatus()
{
    return mPressed;
}

void TrackedElement::operate()
{
    qDebug() << "I am TrackedElement " <<  name;
}
