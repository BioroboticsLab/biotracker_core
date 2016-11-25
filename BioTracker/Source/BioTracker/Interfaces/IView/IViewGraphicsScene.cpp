#include "IViewGraphicsScene.h"

IViewGraphicsScene::IViewGraphicsScene(QObject *parent, IController *controller, IModel *model) :
    QGraphicsScene(parent),
    IView(controller, model)
{

}

void IViewGraphicsScene::setNewModel(IModel *model)
{

}

void IViewGraphicsScene::connectModelView()
{
    QObject::connect(getModel(), &IModel::notifyView, this, &IViewGraphicsScene::getNotified);
}
