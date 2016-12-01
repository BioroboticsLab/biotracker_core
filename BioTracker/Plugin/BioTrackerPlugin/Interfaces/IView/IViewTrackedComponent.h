#ifndef IVIEWTRACKEDCOMPONENT_H
#define IVIEWTRACKEDCOMPONENT_H

#include "IView.h"
#include "QGraphicsObject"

class IViewTrackedComponent : public QGraphicsObject, public IView
{
    Q_OBJECT
public:
    IViewTrackedComponent(QGraphicsItem *parent = 0, IController *controller = 0, IModel *model = 0);
    ~IViewTrackedComponent();

    // IView interface
public:
    void setNewModel(IModel *model) override;

protected:
    void connectModelView() override;

public Q_SLOTS:
  virtual void getNotified() = 0;

};

#endif // IVIEWTRACKEDCOMPONENT_H
