/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/
#pragma once

#include "IControllerCfg.h"

/**
 * The null_Controller class is for initialisation of IController classes instead of using Null Pointers.
 */
class null_Controller : public IControllerCfg {
  public:
    null_Controller();

    // IController interface
  protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
    void connectControllerToController() override;
};
