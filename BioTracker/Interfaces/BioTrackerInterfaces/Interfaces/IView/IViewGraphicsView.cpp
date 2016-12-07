#include "IViewGraphicsView.h"


IViewGraphicsView::IViewGraphicsView(QWidget *parent, IController *controller, IModel *model) :
    QGraphicsView(parent),
    IView(controller, model)
{

}

void IViewGraphicsView::setNewModel(IModel *model)
{

}

void IViewGraphicsView::connectModelView()
{
    QObject::connect(getModel(), &IModel::notifyView, this, &IViewGraphicsView::getNotified);
}
