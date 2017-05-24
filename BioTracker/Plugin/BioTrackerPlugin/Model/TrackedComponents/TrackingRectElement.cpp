#include "TrackingRectElement.h"


void TrackingRectElement::pressed()
{
	_pressed = true;
	Q_EMIT notifyView();

}

void TrackingRectElement::notPressed()
{
	_pressed = false;
	Q_EMIT notifyView();
}

