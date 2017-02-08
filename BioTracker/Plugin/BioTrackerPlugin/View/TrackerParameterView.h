#ifndef TRACKERPARAMETERVIEW_H
#define TRACKERPARAMETERVIEW_H

#include "Interfaces/IView/IViewWidget.h"
#include "Model/TrackerParameter.h"

namespace Ui {
class TrackerParameterView;
}

class TrackerParameterView : public IViewWidget
{
    Q_OBJECT

public:
    explicit TrackerParameterView(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);
    ~TrackerParameterView();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TrackerParameterView *ui;

    // IViewWidget interface
public slots:

    void getNotified();
};

#endif // TRACKERPARAMETERVIEW_H
