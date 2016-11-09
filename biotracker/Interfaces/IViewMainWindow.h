#ifndef IVIEWMAINWINDOW_H
#define IVIEWMAINWINDOW_H

#include <QMainWindow>
#include "Interfaces/IView.h"

class IViewMainWindow : public QMainWindow, public IView {
  public:
    IViewMainWindow(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);
    ~IViewMainWindow();

  private:
    void setNewModel(IModel *model);
    void connectModelView();
};

#endif // IVIEWMAINWINDOW_H
