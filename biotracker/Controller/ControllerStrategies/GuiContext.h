#ifndef GUICONTEXT_H
#define GUICONTEXT_H

#include "Interfaces/IBioTrackerContext.h"
#include "Interfaces/IController/icontroller.h"

class GuiContext : public IBioTrackerContext
{
    Q_OBJECT

public:
    GuiContext(QObject *parent = 0);

    // IBioTrackerContext interface
protected:
    void createAppController() override;
    void connectController() override;

};

#endif // GUICONTEXT_H
