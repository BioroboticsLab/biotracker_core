#include "IGraphicsView.h"

IGraphicsView::IGraphicsView(QWidget *parent, IController *controller, IModel *model) :
    QGraphicsView(parent),
    IView(controller, model)
{

}

void IGraphicsView::setNewModel(IModel *model)
{

}

void IGraphicsView::connectModelView()
{
    QObject::connect(getModel(), &IModel::notifyView, this, &IGraphicsView::getNotified);
}
