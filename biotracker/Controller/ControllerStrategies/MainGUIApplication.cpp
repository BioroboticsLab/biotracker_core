#include "MainGUIApplication.h"
#include "../biotracker_gui/biotracker/View/BioTracker3MainWindow.h"

MainGUIApplication::MainGUIApplication()
{
    createMainWindow();
    createTextureObject();
}

void MainGUIApplication::execute()
{

}

void MainGUIApplication::createMainWindow()
{
    m_MainWindow = new BioTracker3MainWindow(0, m_Controller);
    static_cast<BioTracker3MainWindow *>(m_MainWindow)->show();
}

void MainGUIApplication::createTextureObject()
{

}
