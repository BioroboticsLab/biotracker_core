/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef NULL_CONTROLLER_H
#define NULL_CONTROLLER_H

#include "Interfaces/IController/IController.h"
#include "Interfaces/ENUMS.h"
#include "util/Config.h"

/**
 * The null_Controller class is for initialisation of IController classes instead of using Null Pointers.
 */
class IControllerCfg : public IController {

public:
  IControllerCfg(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

	void setConfig(Config *cfg);

	Config *getConfig();

protected:
	Config *_cfg;
};

#endif // NULL_CONTROLLER_H
