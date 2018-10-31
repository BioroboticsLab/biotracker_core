#pragma once
#include "Interfaces/IModel/IModelAreaDescriptor.h"
#include "Interfaces/IModel/IModel.h"
#include "AreaInfoElement.h"
#include "util/types.h"
#include "Model/MediaPlayerStateMachine/PlayerParameters.h"

namespace AreaMemory
{
#define DEFAULT_RECT        "15,15;15,105;105,105;105,15"
#define DEFAULT_AREA        "10,10;10,100;100,100;100,10"
#define DEFAULT_PAIR        QVector<QString>{DEFAULT_AREA, DEFAULT_RECT, "0", "0"}

    QVector<QString> getVertices(QString file, QString areaFile, bool useLast = false);
    void setVertices(QString file, QVector<QString> values, QString areaFile);
    std::vector<QPoint> toQPointVector(QString vertices);
};

