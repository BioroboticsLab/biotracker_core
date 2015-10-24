#pragma once

#include <stdlib.h>
#include <czmq.h>

namespace BioTracker {
namespace Core {
namespace Interpreter {

class Interpreter : public QObject {

  public:
    Interpreter() {}
    ~Interpreter() {}
    size_t interpret() {

        return 87;
    }

};

}
}
}
