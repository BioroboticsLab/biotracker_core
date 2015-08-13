#pragma once

#include <QDialog>

#include "biotracker/core/BioTrackerApp.h"
#include "biotracker/gui/ui_OpenCameraDialog.h"
#include "biotracker/util/QtRaiiWrapper.hpp"

namespace BioTracker {
namespace Gui {

class OpenCameraDialog : public QDialog {
    Q_OBJECT
  public:
    typedef Util::QtRaiiWrapper<Ui::OpenCameraDialog, QDialog> OpenCameraDialogUi;

    explicit OpenCameraDialog(QWidget *parent, Core::BioTrackerApp &facade);
    OpenCameraDialogUi &getUi() {
        return m_ui;
    }

  private:
    OpenCameraDialogUi m_ui;
    Core::BioTrackerApp &m_facade;

    void initConnects();

  private slots:
};

}
}
