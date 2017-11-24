#pragma once
#include <vector>
#include <algorithm>
#include <qobject.h>
#include <QList>
#include <QObject>

class Serializable : public QObject
{
	Q_OBJECT
};

QDataStream &operator<<(QDataStream &out, const Serializable &data);
QDataStream &operator>>(QDataStream &in, Serializable &data);
