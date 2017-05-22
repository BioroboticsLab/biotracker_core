/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef NULL_CONTROLLER_H
#define NULL_CONTROLLER_H

#include "Interfaces/IController/IController.h"

/**
 * The null_Controller class is for initialisation of IController classes instead of using Null Pointers.
 */
class null_Controller : public IController {
  public:
    null_Controller();

    // IController interface
  protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
    void connectControllerToController() override;
};

#endif // NULL_CONTROLLER_H
