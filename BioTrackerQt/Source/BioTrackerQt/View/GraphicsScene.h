#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "Interfaces/IView/IViewGraphicsScene.h"

class GraphicsScene : public IViewGraphicsScene
{
public:
    GraphicsScene(QObject *parent = 0, IController *controller = 0, IModel *model = 0);

    // IViewGraphicsScene interface
public Q_SLOTS:
    void getNotified() override;
};

#endif // GRAPHICSSCENEVIEW_H
