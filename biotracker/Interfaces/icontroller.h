#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <QObject>
#include "../biotracker_gui/biotracker/BioTracker3MainWindow.h"
#include "iview.h"
#include "imodel.h"

class IController : public QObject {
    Q_OBJECT
  public:
    explicit IController(QObject *parent = 0);
    void registerMainBioTrackerWindow(BioTracker3MainWindow *w);

    IController *getController();

  protected:
    void addView(IView *view);
    void addModel(IModel *model);
    IModel *getModel(int n);
    IView *getView(int n);
    BioTracker3MainWindow *getMainWindow();

  Q_SIGNALS:

  public Q_SLOTS:

  private:
    // MVC
    BioTracker3MainWindow *mMainWindow;
    QList<IView *> *mViews;
    QList<IModel *> *mModels;

};

#endif // ICONTROLLER_H
