#include "TrackedElement.h"
#include "QDebug"

TrackedElement::TrackedElement(QObject *parent, QString name) :
    ITrackedComponent(parent),
    name(name)
{
    x = 0;
    y = 0;
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
