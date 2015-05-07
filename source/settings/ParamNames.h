#pragma once

namespace CONFIGPARAM
{
static const std::string CONFIGURATION_FILE = "./config.json";
static const std::string GEOMETRY_FILE      = "./geometry.cfg";
static const std::string STATE_FILE         = "./state.cfg";
}

namespace TRACKERPARAM {
static const std::string TRACKING_ENABLED = "TRACKERPARAM.TRACKING_ENABLED";

static const std::string SELECTED_TRACKER = "TRACKERPARAM.SELECTED_TRACKER";
}

namespace GUIPARAM
{
enum class MediaType : uint8_t {
	NoMedia = 0,
	Video,
	Images
};

static const std::string MEDIA_TYPE			= "GUIPARAM.MEDIA_TYPE";
static const std::string SHORTCUT_PLAY		= "GUIPARAM.SHORTCUT_PLAY";
static const std::string SHORTCUT_NEXT		= "GUIPARAM.SHORTCUT_NEXT";
static const std::string SHORTCUT_PREV		= "GUIPARAM.SHORTCUT_PREV";
static const std::string SHORTCUT_ZOOM		= "GUIPARAM.SHORTCUT_ZOOM";

static const std::string PAUSED_AT_FRAME	= "GUIPARAM.PAUSED_AT_FRAME";
}

namespace PICTUREPARAM
{
//path to picture
static const std::string PICTURE_FILES = "PICTUREPARAM.PICTURE_FILES";
}

namespace CAPTUREPARAM
{
// Video path
static const std::string CAP_VIDEO_FILE      = "CAPTUREPARAM.CAP_VIDEO_FILE";
static const std::string CAP_SCREENSHOT_PATH = "CAPTUREPARAM.CAP_SCREENSHOT_PATH";

// Video paths
static const std::string ENABLE_RECORD_RAW        = "CAPTUREPARAM.ENABLE_RECORD_RAW";
static const std::string ENABLE_RECORD_TRACKING   = "CAPTUREPARAM.ENABLE_RECORD_TRACKING";
static const std::string ENABLE_RECORD_TRACK_INFO = "CAPTUREPARAM.ENABLE_RECORD_TRACK_INFO";

static const std::string RECORDING_TRACKING_PATH   = "CAPTUREPARAM.RECORDING_TRACKING_PATH";
static const std::string RECORDING_RAW_PATH        = "CAPTUREPARAM.RECORDING_RAW_PATH";
static const std::string TRACKING_INFO_OUTPUT_PATH = "CAPTUREPARAM.TRACKING_INFO_OUTPUT_PATH";

enum CAP_TYPE_ENUM
{
	CAP_FROM_CAM  = 0,
	CAP_FROM_FILE = 1
};

enum CAP_STATE_ENUM
{
	CAP_FINISHED  = 0,
	CAP_PAUSE     = 1,
	CAP_AVAILABLE = 2,
	CAP_EXITED    = 3
};
}

