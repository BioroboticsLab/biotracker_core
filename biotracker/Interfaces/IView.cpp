#include "IView.h"
#include "Interfaces/icontroller.h"

IView::IView(IController *controller):
    mController(controller) {

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
