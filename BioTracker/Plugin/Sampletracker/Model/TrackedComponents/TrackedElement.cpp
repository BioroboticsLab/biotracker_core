#include "TrackedElement.h"
#include "QDebug"
#include "QRect"
#include "QBrush"
#include "QPainter"

TrackedElement::TrackedElement(QObject *parent, QString name, int id) :
    IModelTrackedComponent(parent),
    _name(name),
	_id(id)
{
    _pressed = false;
}

QString TrackedElement::getName()
{
    return _name;
}

void TrackedElement::pressed()
{
	_pressed = true;
    Q_EMIT notifyView();

}

void TrackedElement::notPressed()
{
	_pressed = false;
    Q_EMIT notifyView();
}

bool TrackedElement::getPressedStatus()
{
    return _pressed;
}

void TrackedElement::operate()
{
    qDebug() << "I am TrackedElement " <<  _name;
}
