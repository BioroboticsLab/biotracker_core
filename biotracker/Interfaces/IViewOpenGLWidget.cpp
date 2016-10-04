#include "IViewOpenGLWidget.h"

IViewOpenGLWidget::IViewOpenGLWidget(QWidget *parent, IController *controller, IModel *model) :
    QOpenGLWidget(parent),
    IView(controller) {
    setModel(model);
}

IViewOpenGLWidget::~IViewOpenGLWidget() {

}

void IViewOpenGLWidget::setNewModel(IModel *model) {
    setModel(model);

    if (getModel() != nullptr) {
        connectModelView();
    }
}

void IViewOpenGLWidget::connectModelView() {
    QObject::connect(getModel(), &IModel::notifyView, this, &IViewOpenGLWidget::getNotified);
}

void IViewOpenGLWidget::initializeGL() {
    connectModelView();
}
