#pragma once

#include <QWidget>

#include <QCamera>
#include "QCameraViewfinder"
#include "QPointer"
#include "util/types.h"
#include <opencv2/opencv.hpp>
#include "ui_SettingsWindow.h"

namespace Ui {
	class SettingsWindow;
}

class SettingsWindow : public QWidget
{
	Q_OBJECT

public:
	explicit SettingsWindow(QWidget *parent = 0);
	~SettingsWindow();

	private Q_SLOTS:
	//void on_buttonBox_accepted();

	//void on_pushButton_clicked();
	void on_buttonSaveClose_clicked();

	void on_pushButtonSetNewLocManSave_clicked();

	//void on_comboBox_currentIndexChanged(int index);

Q_SIGNALS:
	//void emitSelectedCameraDevice(CameraConfiguration conf);

private:

private:
	Ui::SettingsForm *ui;
};