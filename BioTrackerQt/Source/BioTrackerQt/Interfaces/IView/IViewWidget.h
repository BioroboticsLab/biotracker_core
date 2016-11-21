#ifndef IVIEWWIDGET_H
#define IVIEWWIDGET_H

#include <QWidget>
#include "Interfaces/IView/IView.h"

class IViewWidget : public QWidget, public IView {
    Q_OBJECT
  public:
    explicit IViewWidget(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);

    void setNewModel(IModel *model);

  public Q_SLOTS:
    virtual void getNotified() = 0;

  private:
    void connectModelView();
};

#endif // IVIEWWIDGET_H
