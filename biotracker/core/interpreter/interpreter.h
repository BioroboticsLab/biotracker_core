#pragma once
//#include <Python.h>
#include "/usr/include/python3.4/Python.h"

#include <stdlib.h>

namespace BioTracker {
namespace Core {
namespace Interpreter {

class Interpreter : public QObject {

  public:
    Interpreter() {}
    ~Interpreter() {}
    size_t interpret() {

        Py_Initialize();

        PyRun_SimpleString("print('test 123')");

        Py_Finalize();

        return 5;
    }

};

}
}
}
