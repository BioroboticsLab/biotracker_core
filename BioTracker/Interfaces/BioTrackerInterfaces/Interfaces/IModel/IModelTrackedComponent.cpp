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

void IModelTrackedPoint::operate()
{

}

void IModelTrackedPolygon::operate()
{

}
