#pragma once

#include <QDialog>

#include "source/core/Facade.h"
#include "source/gui/ui_OpenCameraDialog.h"
#include "source/util/QtRaiiWrapper.hpp"

namespace BioTracker {
namespace Gui {

class OpenCameraDialog : public QDialog
{
    Q_OBJECT
public:
	typedef Util::QtRaiiWrapper<Ui::OpenCameraDialog, QDialog> OpenCameraDialogUi;

    explicit OpenCameraDialog(QWidget *parent, Core::Facade &facade);

private:
	OpenCameraDialogUi m_ui;
    Core::Facade& m_facade;

    void initConnects();

private slots:
};

}
}
