#include "IView.h"
#include "Interfaces/IController/IController.h"

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

void IView::setPermission(std::pair<ENUMS::COREPERMISSIONS, bool>) {

}
