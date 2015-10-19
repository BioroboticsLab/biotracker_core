#pragma once

#include <stdlib.h>

// http://czmq.zeromq.org/page:get-the-software

namespace BioTracker {
namespace Core {
namespace Interpreter {

class Interpreter : public QObject {

  public:
    Interpreter() {}
    ~Interpreter() {}
    size_t interpret() {

        return 7;
    }

};

}
}
}
