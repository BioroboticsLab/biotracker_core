#include "icontroller.h"

IController::IController(QObject *parent) : QObject(parent) {
    mViews = new QList<IView *>;
    mModels = new QList<IModel *>;
}

void IController::registerMainBioTrackerWindow(BioTracker3MainWindow *w) {
    mMainWindow = w;
    mMainWindow->addController(this);
    mMainWindow->show();
}

IController *IController::getController() {
    return this;
}

void IController::addView(IView *view) {
    mViews->append(view);
}

void IController::addModel(IModel *model) {
    mModels->append(model);
}

IModel *IController::getModel(int n) {
    return mModels->at(n);
}

IView *IController::getView(int n) {
    return mViews->at(n);
}

BioTracker3MainWindow *IController::getMainWindow() {
    return mMainWindow;
}
