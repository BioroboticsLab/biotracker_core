#ifndef IOBJECT_H
#define IOBJECT_H

#include "Interfaces/IModel/IModel.h"

// #include "biotracker/util/platform.h"

class IObject : public IModel {
    Q_OBJECT
  public:
    IObject();
};

#endif // IOBJECT_H
