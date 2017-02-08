#include "TrackerParameterView.h"
#include "ui_TrackerParameterView.h"

TrackerParameterView::TrackerParameterView(QWidget *parent, IController *controller, IModel *model) :
    IViewWidget(parent, controller, model),
    ui(new Ui::TrackerParameterView)
{
    ui->setupUi(this);

    ui->lineEdit->setValidator( new QIntValidator(this));

    getNotified();
}

TrackerParameterView::~TrackerParameterView()
{
    delete ui;
}

void TrackerParameterView::on_pushButton_clicked()
{
    int threshold = ui->lineEdit->text().toInt();

    TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
    parameter->setThreshold(threshold);
}

void TrackerParameterView::getNotified()
{
    TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
    int threshold = parameter->getThreshold();

    ui->lineEdit->setText(QString::number(threshold));
}
