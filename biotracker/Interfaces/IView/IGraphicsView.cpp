#include "IGraphicsView.h"

IGraphicsView::IGraphicsView(QObject *parent, IController *controller, IModel *model) :
    QGraphicsScene(parent),
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
