#include "icontroller.h"

IController::IController(QObject *parent) : QObject(parent) {
    mViews = new QMap<QString, IView *>;
    mModels = new QMap<QString, IModel *>;
}

IController *IController::getController() {
    return this;
}

void IController::addView(QString name, IView *view) {
    mViews->insert(name, view);
}

void IController::addModel(QString name, IModel *model) {
    mModels->insert(name, model);
}

IModel *IController::getModel(QString name) {
    return mModels->value(name);
}

IView *IController::getView(QString name) {
    return mViews->value(name);
}
