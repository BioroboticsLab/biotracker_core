#ifndef MAINGUIAPPLICATION_H
#define MAINGUIAPPLICATION_H

#include "Interfaces/IController/IControllerStrategy.h"
#include "biotracker/Model/BioTracker3TextureObject.h"

class MainGUIApplication : public IControllerStrategy
{
public:
    MainGUIApplication();

    // IControllerStrategy interface
public:
    void execute() override;

private:
    void createMainWindow();
    void createTextureObject();


private:
    IView *m_MainWindow;
    BioTracker3TextureObject *m_TextureObjectModel;

};

#endif // MAINGUIAPPLICATION_H
