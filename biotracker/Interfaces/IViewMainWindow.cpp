#include "IViewMainWindow.h"

IViewMainWindow::IViewMainWindow(QWidget *parent, IController *controller) :
    QMainWindow(parent),
    IView(controller) {

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
