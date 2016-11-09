#ifndef GUICONTEXT_H
#define GUICONTEXT_H

#include "Interfaces/IBioTrackerContext.h"
#include "Interfaces/icontroller.h"

class GuiContext : public IBioTrackerContext
{
    Q_OBJECT
public:
    GuiContext(QObject *parent = 0);

    // IBioTrackerContext interface
protected:
    void createAppController() override;
    void connectController() override;

private:
    IController *m_MainWindowController;
    IController *m_TextureObjectController;
    IController *m_PlayerController;
    IController *m_TrackerController;
    IController *m_TrackedComponentController;

};

#endif // GUICONTEXT_H
