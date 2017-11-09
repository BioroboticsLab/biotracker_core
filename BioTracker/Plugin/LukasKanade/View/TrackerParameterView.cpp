#include "TrackerParameterView.h"
#include "ui_TrackerParameterView.h"

#include <iostream>

TrackerParameterView::TrackerParameterView(QWidget *parent, IController *controller, IModel *model) :
    IViewWidget(parent, controller, model),
    ui(new Ui::TrackerParameterView)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEdit_wndSize, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_clicked()));
    getNotified();
}

TrackerParameterView::~TrackerParameterView()
{
    delete ui;
}

void TrackerParameterView::on_pushButton_clicked()
{
    TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());

    int setWndSize = ui->lineEdit_wndSize->text().toInt();

    parameter->setAll(setWndSize);

    Q_EMIT parametersChanged();
}

void TrackerParameterView::getNotified()
{
    TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());

	int val = parameter->getWndSize();
	ui->lineEdit_wndSize->setValue(val);
}
