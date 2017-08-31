#include "TrackerParameterView.h"
#include "ui_TrackerParameterView.h"
#include "Model/TrackerParameter.h"

#include <iostream>

namespace sampleTracker {
	TrackerParameterView::TrackerParameterView(QWidget *parent, IController *controller, IModel *model) :
		IViewWidget(parent, controller, model),
		ui(new Ui::TrackerParameterView)
	{
		ui->setupUi(this);
		
		//ui->lineEdit->setValidator(new QIntValidator(this));
		ui->lineEdit_2_lowH->setValidator(new QIntValidator(this));
		ui->lineEdit_3_highH->setValidator(new QIntValidator(this));
		ui->lineEdit_4_lowS->setValidator(new QIntValidator(this));
		ui->lineEdit_5_HighS->setValidator(new QIntValidator(this));
		ui->lineEdit_6_LowV->setValidator(new QIntValidator(this));
		ui->lineEdit_7_HighV->setValidator(new QIntValidator(this));

		getNotified();
	}

	TrackerParameterView::~TrackerParameterView()
	{
		delete ui;
	}


	void TrackerParameterView::on_pushButton_clicked()
	{
		//int threshold = ui->lineEdit->text().toInt();
		TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
		//parameter->setThreshold(threshold);
		
		int lowh = ui->lineEdit_2_lowH->text().toInt();
		int highh = ui->lineEdit_3_highH->text().toInt();
		int lows = ui->lineEdit_4_lowS->text().toInt();
		int highs = ui->lineEdit_5_HighS->text().toInt();
		int lowv = ui->lineEdit_6_LowV->text().toInt();
		int highv = ui->lineEdit_7_HighV->text().toInt();

		//parameter->setAll(lowh, highh, lows, highs, lowv, highv);
	}

	void TrackerParameterView::getNotified()
	{
		TrackerParameter *parameter = qobject_cast<TrackerParameter *>(getModel());
		//int threshold = parameter->getThreshold();
		//ui->lineEdit->setText(QString::number(threshold));
		
		int val = parameter->_lowH;
		ui->lineEdit_2_lowH->setText(QString::number(val));

		val = parameter->_highH;
		ui->lineEdit_3_highH->setText(QString::number(val));

		val = parameter->_lowS;
		ui->lineEdit_4_lowS->setText(QString::number(val));

		val = parameter->_highS;
		ui->lineEdit_5_HighS->setText(QString::number(val));

		val = parameter->_lowV;
		ui->lineEdit_6_LowV->setText(QString::number(val));

		val = parameter->_highV;
		ui->lineEdit_7_HighV->setText(QString::number(val));
	}
}