#include "IViewOpenGLWidget.h"

IViewOpenGLWidget::IViewOpenGLWidget(QWidget *parent, IController *controller, IModel *model) :
    QOpenGLWidget(parent),
    IView(controller) {
    setNewModel(model);
}

IViewOpenGLWidget::~IViewOpenGLWidget() {

}

void IViewOpenGLWidget::setNewModel(IModel *model) {    
    if (getModel() != nullptr) {
        QObject::disconnect(getModel(), 0, this, 0);
    }

    setModel(model);

    if (getModel() != nullptr) {
        connectModelView();
    }

    update();
}

void IViewOpenGLWidget::connectModelView() {
    QObject::connect(getModel(), &IModel::notifyView, this, &IViewOpenGLWidget::getNotified);
}

void IViewOpenGLWidget::initializeGL() {
    connectModelView();
}
