#ifndef GUICONTEXT_H
#define GUICONTEXT_H

#include "Interfaces/IBioTrackerContext.h"
#include "Interfaces/IController/IController.h"
#include "util/Config.h"

class GuiContext : public IBioTrackerContext
{
    Q_OBJECT

public:
    GuiContext(QObject *parent = 0, Config *cfg = nullptr);
	~GuiContext() {
		exit();
	}

    void loadBioTrackerPlugin(QString str);

	void exit();

    // IBioTrackerContext interface
protected:
    void createAppController() override;
    void connectController() override;
	Config *_cfg;
};

#endif // GUICONTEXT_H
