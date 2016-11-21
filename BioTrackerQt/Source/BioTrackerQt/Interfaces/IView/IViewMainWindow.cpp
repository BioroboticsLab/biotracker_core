#include "IViewMainWindow.h"

IViewMainWindow::IViewMainWindow(QWidget *parent, IController *controller, IModel *model) :
    QMainWindow(parent),
    IView(controller, model) {

}

IViewMainWindow::~IViewMainWindow() {

}

void IViewMainWindow::setNewModel(IModel *model) {
    model = nullptr;
    setModel(model);
}

void IViewMainWindow::connectModelView() {
    // this function is not used in MainWindow
}
