#include "IModelTrackedComponent.h"
#include <qvariant.h>
#include <qmetaobject.h>
#include "Interfaces/IModel/IModelTrackedComponentFactory.h"
#include "IModelTrackedTrajectory.h"

IModelTrackedComponentFactory* factory = nullptr;

IModelTrackedComponent::IModelTrackedComponent(QObject *parent) :
	IModel(parent)
{

}

IModelComponentEuclidian2D::IModelComponentEuclidian2D(QObject *parent) :
	IModelTrackedComponent(parent)
{

}

IModelComponentTemporal2D::IModelComponentTemporal2D(QObject *parent) :
    IModelComponentEuclidian2D(parent)
{

}

IModelTrackedPoint::IModelTrackedPoint(QObject *parent) :
    IModelComponentTemporal2D(parent)
{

}

IModelTrackedPolygon::IModelTrackedPolygon(QObject *parent) :
    IModelComponentTemporal2D(parent)
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

QDataStream &operator<<(QDataStream &ds, const QList<IModelTrackedComponent*> &data) {
    ds << qint64(data.size());
    for (int i = 0; i<data.size(); ++i)
    {
        if (data[i] != nullptr)
            ds << QString((data[i])->metaObject()->className());
        else
            ds << QString("NULL");
        ds << *(data[i]);
    }
    return ds;
}

QDataStream &operator >> (QDataStream &ds, QList<IModelTrackedComponent*> &data) {
    qint64 s;
    ds >> s;
    for (int i = 0; i<s; ++i)
    {
        QString cn;
        ds >> cn;
        std::string sss = cn.toStdString();
        IModelTrackedComponent* cp = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement(cn));
        ds >> (*cp);
        data.append(cp);
    }
    return ds;
}

QDataStream &operator<<(QDataStream &ds, const IModelTrackedComponent &data) {
	for (int i = 0; i<data.metaObject()->propertyCount(); ++i)
	{
		if (data.metaObject()->property(i).isStored(&data))
		{
            QVariant v = data.metaObject()->property(i).read(&data);
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

