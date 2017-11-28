#include "IModelTrackedComponent.h"
#include <qvariant.h>
#include <qmetaobject.h>

IModelTrackedComponent::IModelTrackedComponent(QObject *parent) :
	IModel(parent)
{

}

IModelComponentEuclidian2D::IModelComponentEuclidian2D(QObject *parent) :
	IModelTrackedComponent(parent)
{

}

IModelTrackedPoint::IModelTrackedPoint(QObject *parent) :
	IModelComponentEuclidian2D(parent)
{

}

IModelTrackedPolygon::IModelTrackedPolygon(QObject *parent) :
	IModelComponentEuclidian2D(parent)
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

QDataStream &operator<<(QDataStream &ds, const IModelTrackedComponent &data) {
	for (int i = 0; i<data.metaObject()->propertyCount(); ++i)
	{
		if (data.metaObject()->property(i).isStored(&data))
		{
			ds << data.metaObject()->property(i).read(&data);
		}
	}
	return ds;
}

QDataStream &operator>>(QDataStream &ds, IModelTrackedComponent &data) {
	QVariant var;
	for (int i = 0; i<data.metaObject()->propertyCount(); ++i)
	{
		if (data.metaObject()->property(i).isStored(&data))
		{
			ds >> var;
			data.metaObject()->property(i).write(&data, var);
		}
	}
	return ds;
}

