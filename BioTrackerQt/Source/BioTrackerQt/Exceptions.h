#pragma once

#include <stdexcept>


namespace BioTracker {
namespace Core {

struct video_open_error : std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

struct file_not_found : std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

struct directory_not_found : std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

struct device_open_error : std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

struct invalid_tracker_lib_error : std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

struct path_creation_error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

}
}
