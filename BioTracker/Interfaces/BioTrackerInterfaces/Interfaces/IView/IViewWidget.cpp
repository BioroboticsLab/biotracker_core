#include "IViewWidget.h"

IViewWidget::IViewWidget(QWidget *parent, IController *controller, IModel *model):
    QWidget(parent),
    IView(controller) {
    setNewModel(model);
}

void IViewWidget::setNewModel(IModel *model) {
    setModel(model);
    if (getModel() != nullptr) {
        connectModelView();
    }
}

void IViewWidget::connectModelView() {
    QObject::connect(getModel(), &IModel::notifyView, this, &IViewWidget::getNotified);
}
