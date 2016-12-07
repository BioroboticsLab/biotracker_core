#ifndef CONTROLLERGRAPHICSCENE_H
#define CONTROLLERGRAPHICSCENE_H

#include "Interfaces/IController/IController.h"
#include "QGraphicsObject"
#include "QPointer"


class ControllerGraphicScene : public IController
{
    Q_OBJECT
public:
    ControllerGraphicScene(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void addGraphicsItem(QGraphicsItem *item);
    void addTextureObject(QGraphicsItem *item);

    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
    void connectControllerToController() override;

private:
    IView *m_GraphicsView;

    IModel *m_Element;
    QPointer< IModel > m_NullModel;
};

#endif // CONTROLLERGRAPHICSCENE_H
