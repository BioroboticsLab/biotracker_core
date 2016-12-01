#ifndef IVIEW_H
#define IVIEW_H

#include "Interfaces/IModel/imodel.h"

class IController;
class IView {

  public:
    IView(IController *controller = 0, IModel *model = 0);
    virtual ~IView() = 0;

    virtual void setNewModel(IModel *model) = 0;

  protected:
    virtual void connectModelView() = 0;
    void setModel(IModel *model);
    IController *getController();
    IModel *getModel();

  private:
    IController *mController;
    IModel *mModel;

};

#endif // IVIEW_H
