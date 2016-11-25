#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

class ENUMS : public QObject
{
    Q_OBJECT
public:


    enum CONTROLLERTYPE {NO_CTR, MAINWINDOW, TEXTUREOBJECT, PLAYER, TRACKING, COMPONENT, GRAPHICSVIEW };
    Q_ENUM(CONTROLLERTYPE)

    explicit ENUMS(QObject *parent = 0);

};

#endif // ENUMS_H
