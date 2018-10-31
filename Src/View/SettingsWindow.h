#pragma once

#include <QWidget>

#include <QCamera>
#include "QCameraViewfinder"
#include "QPointer"
#include "util/types.h"
#include <opencv2/opencv.hpp>
#include "ui_SettingsWindow.h"
#include "Controller/IControllerCfg.h"

namespace Ui {
	class SettingsWindow;
}

class SettingsWindow : public QWidget
{
	Q_OBJECT

public:
	explicit SettingsWindow(QWidget *parent, Config *cfg);
	~SettingsWindow();

private Q_SLOTS:
	void on_buttonSaveClose_clicked();
	void on_pushButtonSetNewLocManSave_clicked();
private:
	Ui::SettingsForm *ui;
	Config *_cfg;
};