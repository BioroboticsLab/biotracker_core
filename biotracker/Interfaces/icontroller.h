#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <QObject>
#include "../BioTrackerGui/biotracker/View/BioTracker3MainWindow.h"
#include "IViewWidget.h"
#include "imodel.h"
#include "QMap"

class IController : public QObject {
    Q_OBJECT
  public:
    explicit IController(QObject *parent = 0);

    virtual void createApplication() = 0;
    IController *getController();

  protected:
    void addView(QString name, IView *view);
    void addModel(QString name, IModel *model);
    IModel *getModel(QString name);
    IView *getView(QString name);

  private:
    QMap<QString, IView *> *mViews;
    QMap<QString, IModel *> *mModels;
};

#endif // ICONTROLLER_H
