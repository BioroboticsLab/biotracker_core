#ifndef CONTROLLERGRAPHICSCENE_H
#define CONTROLLERGRAPHICSCENE_H

#include "Interfaces/IController/icontroller.h"

class ControllerGraphicScene : public IController
{
public:
    ControllerGraphicScene(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectController() override;

private:
    IView *m_ElementView;
    IView *m_GraphicsView;

    IModel *m_Element;
    IModel *m_NullModel;
};

#endif // CONTROLLERGRAPHICSCENE_H
