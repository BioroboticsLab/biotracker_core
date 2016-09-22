#ifndef IVIEW_H
#define IVIEW_H

#include <QWidget>
#include "imodel.h"

class IController;
class IView : public QWidget {
    Q_OBJECT
  public:
    explicit IView(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);

    virtual void setModel(IModel *model) = 0;

  protected:
    IController *getController();
    IModel *getModel();
    virtual void setupView() = 0;

  Q_SIGNALS:

  public Q_SLOTS:

    virtual void getNotified() = 0;

  protected:
    IModel *mModel;

  private:
    IController *mController;
};

#endif // IVIEW_H
