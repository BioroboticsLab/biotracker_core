#ifndef Messages_h
#define Messages_h

namespace MSGS
{
namespace SYSTEM
{
static const std::string APPLICATION_CANNOT_START   = "Application cannot start";
static const std::string NO_OPENGL                  = "This system has no OpenGL support!";
static const std::string MISSING_CONFIGURATION_FILE = "Configuration file is missing! Using default parameters.";
}

enum MTYPE
{
	NOTIFICATION,
	WARNING,
	FAIL
};
}
#endif // !Messages_h

