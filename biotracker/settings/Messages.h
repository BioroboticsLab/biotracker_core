#ifndef Messages_h
#define Messages_h

namespace MSGS {
namespace SYSTEM {
static const std::string APPLICATION_CANNOT_START   =
    "Application cannot start";
static const std::string NO_OPENGL                  =
    "This system has no OpenGL support!";
static const std::string MISSING_CONFIGURATION_FILE =
    "Configuration file is missing! Using default parameters.";
}

enum MTYPE {
    NOTIFICATION=0,
    WARNING=1,
    FAIL=2,
    FILE_OPEN=3
};

inline MTYPE fromInt(const size_t v) {
    return static_cast<MTYPE>(v);
}

}
#endif // !Messages_h

