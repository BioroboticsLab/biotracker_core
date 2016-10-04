#ifndef IVIEW_H
#define IVIEW_H

#include "imodel.h"

class IController;
class IView {
  public:
    IView(IController *controller = 0);
    virtual ~IView() = 0;

    virtual void setNewModel(IModel *model) = 0;

  protected:
    virtual void connectModelView() = 0;
    void setModel(IModel *model);
    IController *getController();
    IModel *getModel();

  private:
    IModel *mModel;
    IController *mController;
};

#endif // IVIEW_H
