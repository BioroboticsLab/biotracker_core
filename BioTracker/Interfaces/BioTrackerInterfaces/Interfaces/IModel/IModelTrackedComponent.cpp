#include "IModelTrackedComponent.h"

IModelTrackedComponent::IModelTrackedComponent(QObject *parent) :
	IModel(parent)
{

}

IModelTrackedPoint::IModelTrackedPoint(QObject *parent) :
	IModelTrackedComponent(parent)
{

}

IModelTrackedPolygon::IModelTrackedPolygon(QObject *parent) :
	IModelTrackedComponent(parent)
{

}

IModelTrackedEllipse::IModelTrackedEllipse(QObject *parent) :
	IModelTrackedPoint(parent)
{

}

IModelTrackedRectangle::IModelTrackedRectangle(QObject *parent) :
	IModelTrackedPoint(parent)
{

}

void IModelTrackedPoint::operate()
{

}

void IModelTrackedPolygon::operate()
{

}

void IModelTrackedEllipse::operate()
{

}

void IModelTrackedRectangle::operate()
{

}
