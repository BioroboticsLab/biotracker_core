#include "iview.h"
#include "Interfaces/icontroller.h"

IView::IView(QWidget *parent, IController *controller, IModel *model): QWidget(parent) {
    mModel = model;
    mController = controller;

    QObject::connect(mModel, SIGNAL(notifyView()), this, SLOT(getNotified()));
}

IController *IView::getController() {
    return mController;
}

IModel *IView::getModel() {
    return mModel;
}
