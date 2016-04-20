#pragma once

#include <string> // std::string

#include <boost/filesystem.hpp>

#include <QDir>
#include <QStandardPaths>

namespace ConfigParam {
static const boost::filesystem::path CONFIG_PATH = boost::filesystem::path(
            QDir::toNativeSeparators(
                QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)).toStdString());
static const boost::filesystem::path BASE_PATH          = CONFIG_PATH / "BioTracker";
static const boost::filesystem::path CONFIGURATION_FILE = BASE_PATH / "config.json";
static const boost::filesystem::path GEOMETRY_FILE      = BASE_PATH / "geometry.cfg";
static const boost::filesystem::path STATE_FILE         = BASE_PATH / "state.cfg";
static const boost::filesystem::path MODULE_PATH        = BASE_PATH / "modules";
}

namespace TrackerParam {
static const std::string TRACKING_ENABLED = "TRACKERPARAM.TRACKING_ENABLED";

static const std::string SELECTED_TRACKER = "TRACKERPARAM.SELECTED_TRACKER";
}

namespace GuiParam {
enum class MediaType : uint8_t {
    NoMedia = 0,
    Video,
    Images,
    Camera
};

enum class VideoMode : uint8_t {
    Playing = 0,
    Paused,
    Stopped,
    Init
};


static const std::string MEDIA_TYPE         = "GUIPARAM.MEDIA_TYPE";
static const std::string SHORTCUT_PLAY      = "GUIPARAM.SHORTCUT_PLAY";
static const std::string SHORTCUT_NEXT      = "GUIPARAM.SHORTCUT_NEXT";
static const std::string SHORTCUT_PREV      = "GUIPARAM.SHORTCUT_PREV";
static const std::string SHORTCUT_ZOOM      = "GUIPARAM.SHORTCUT_ZOOM";
static const std::string SHORTCUT_TRACKING      = "GUIPARAM.SHORTCUT_TRACKING";

static const std::string PAUSED_AT_FRAME    = "GUIPARAM.PAUSED_AT_FRAME";
}

namespace PictureParam {
//path to picture
static const std::string PICTURE_FILES = "PICTUREPARAM.PICTURE_FILES";
}

namespace CaptureParam {
// Video path
static const std::string CAP_VIDEO_FILE      = "CAPTUREPARAM.CAP_VIDEO_FILE";
static const std::string CAP_SCREENSHOT_PATH =
    "CAPTUREPARAM.CAP_SCREENSHOT_PATH";
static const std::string CAP_CAMERA_ID       = "CAPTUREPARAM.CAP_CAMERA_ID";

// Video paths
static const std::string ENABLE_RECORD_RAW        =
    "CAPTUREPARAM.ENABLE_RECORD_RAW";
static const std::string ENABLE_RECORD_TRACKING   =
    "CAPTUREPARAM.ENABLE_RECORD_TRACKING";
static const std::string ENABLE_RECORD_TRACK_INFO =
    "CAPTUREPARAM.ENABLE_RECORD_TRACK_INFO";

static const std::string RECORDING_TRACKING_PATH   =
    "CAPTUREPARAM.RECORDING_TRACKING_PATH";
static const std::string RECORDING_RAW_PATH        =
    "CAPTUREPARAM.RECORDING_RAW_PATH";
static const std::string TRACKING_INFO_OUTPUT_PATH =
    "CAPTUREPARAM.TRACKING_INFO_OUTPUT_PATH";

enum CAP_TYPE_ENUM {
    CAP_FROM_CAM  = 0,
    CAP_FROM_FILE = 1
};

enum CAP_STATE_ENUM {
    CAP_FINISHED  = 0,
    CAP_PAUSE     = 1,
    CAP_AVAILABLE = 2,
    CAP_EXITED    = 3
};
}

