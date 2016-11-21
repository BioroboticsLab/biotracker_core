#ifndef CAMERADEVICESELECT_H
#define CAMERADEVICESELECT_H

#include <QDialog>

namespace Ui {
class CameraDeviceSelect;
}

class CameraDeviceSelect : public QDialog
{
    Q_OBJECT

public:
    explicit CameraDeviceSelect(QWidget *parent = 0);
    ~CameraDeviceSelect();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CameraDeviceSelect *ui;
};

#endif // CAMERADEVICESELECT_H
