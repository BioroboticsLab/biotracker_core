#include "TrackedElement.h"
#include "QDebug"
#include "QRect"
#include "QBrush"
#include "QPainter"

TrackedElement::TrackedElement(QObject *parent, QString name) :
    ITrackedComponent(parent),
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
}

void TrackedElement::setY(int val)
{
    y = val;
}

void TrackedElement::pressed()
{
    mPressed = true;
}

void TrackedElement::notPressed()
{
    mPressed = false;
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
