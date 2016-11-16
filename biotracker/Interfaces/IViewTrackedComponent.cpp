#include "IViewTrackedComponent.h"

IViewTrackedComponent::IViewTrackedComponent(QGraphicsItem *parent, IController *controller, IModel *model) :
    QGraphicsObject(parent),
    IView(controller, model)
{

}

IViewTrackedComponent::~IViewTrackedComponent()
{

}

void IViewTrackedComponent::setNewModel(IModel *model)
{

}

void IViewTrackedComponent::connectModelView()
{

}
