#pragma once
// tracker
namespace BioTracker {
namespace Core {
/**
 * @brief The TrackerStatus enum
 * describes the current status of the tracking algorithm
 */
enum class TrackerStatus {
    NothingLoaded,  ///< No media selected
    Running,        ///< The tracker is running
    Paused,         ///< The tracker is paused. The calculation of the current frame might still be running
    Invalid         ///< The replayed file is invalid
};
}
}
