/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef NULL_MODEL_H
#define NULL_MODEL_H

#include "Interfaces/IModel/IModel.h"

/**
 * The null_Model class is only for initialization of IModel classes.
 */
class null_Model : public IModel {
    Q_OBJECT
  public:
    null_Model();
};

#endif // NULL_MODEL_H
