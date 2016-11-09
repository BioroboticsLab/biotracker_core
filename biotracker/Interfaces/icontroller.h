#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <QObject>
#include "../biotracker_gui/biotracker/View/BioTracker3MainWindow.h"
#include "IViewWidget.h"
#include "imodel.h"
#include "QMap"
#include "Interfaces/IBioTrackerContext.h"

class IController : public QObject {
    Q_OBJECT
  public:
    explicit IController(QObject *parent = 0, IBioTrackerContext *context = 0);

    void createComponents();

    virtual void connectViewToMainWindow(IController *controller);
    virtual void connectToOtherController(IController *controller);

    void addView(IView *view);
    void addModel(IModel *model);
    IModel *getModel();
    IView *getView();

    IBioTrackerContext *getBioTrackerContext();

protected:
    virtual void createModel() = 0;
    virtual void createView() = 0;
    virtual void connectModelController() = 0;
    virtual void connectModelView() = 0;

protected:
    IController *m_partnerController;

    IBioTrackerContext *m_BioTrackerContext;

    IView *m_View;
    IModel *m_Model;

};

#endif // ICONTROLLER_H
