#ifndef CONTROLLERGRAPHICSCENE_H
#define CONTROLLERGRAPHICSCENE_H

#include "Interfaces/IController/icontroller.h"
#include "QGraphicsObject"
#include "View/TrackedElementView.h"
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
    void connectModelController() override;
    void connectController() override;

private:
    QPointer< TrackedElementView > m_ElementView;
    IView *m_GraphicsView;

    QPointer< IModel > m_Element;
    QPointer< IModel > m_NullModel;
};

#endif // CONTROLLERGRAPHICSCENE_H
