#pragma once

namespace BioTracker {
namespace Core {

namespace Messages {
namespace System {

static const std::string APPLICATION_CANNOT_START   =
    "Application cannot start";
static const std::string NO_OPENGL                  =
    "This system has no OpenGL support!";
static const std::string MISSING_CONFIGURATION_FILE =
    "Configuration file is missing! Using default parameters.";

}

enum class MessageType : size_t {
    NOTIFICATION = 0,
    WARNING,
    FAIL,
    FILE_OPEN
};

inline MessageType fromInt(const size_t v) {
    return static_cast<MessageType>(v);
}

}

using Messages::MessageType;

}
}

