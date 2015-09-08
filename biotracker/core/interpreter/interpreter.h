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

        //std::string name = "test";
        //Py_SetProgramName(name.c_str());

        Py_Initialize();

        PyRun_SimpleString("print('test 123')");

        Py_Finalize();

        return 5;
    }

};

}
}
}
