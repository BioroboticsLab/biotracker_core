#pragma once

#include <QMainWindow>

#include "source/gui/ui_MainWindow.h"

namespace BioTracker {
namespace Gui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

    const Ui::MainWindowUi& getUi() const { return m_ui; }

private:
    Ui::MainWindowUi m_ui;
};



}
}
