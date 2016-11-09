#include "IView.h"
#include "Interfaces/icontroller.h"

IView::IView(IController *controller, IModel *model):
    mController(controller),
    mModel(model)
{

}

void IView::setModel(IModel *model) {
    mModel = model;
}

IView::~IView() {

}

IController *IView::getController() {
    return mController;
}

IModel *IView::getModel() {
    return mModel;
}
