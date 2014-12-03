#pragma once

namespace CONFIGPARAM
{
static const std::string CONFIGURATION_FILE = "./config.json";
}

namespace TRACKERPARAM {
static const std::string TRACKING_ENABLED = "TRACKERPARAM.TRACKING_ENABLED";
}

namespace GUIPARAM
{
// true if video is loaded, false if picture is loaded
static const std::string IS_SOURCE_VIDEO = "GUIPARAM.IS_SOURCE_VIDEO";
}

namespace PICTUREPARAM
{
//path to picture
static const std::string PICTURE_FILE = "PICTUREPARAM.PICTURE_FILE";
}

namespace CAPTUREPARAM
{
// Video path
static const std::string CAP_VIDEO_FILE      = "CAPTUREPARAM.CAP_VIDEO_FILE";
static const std::string CAP_SCREENSHOT_PATH = "CAPTUREPARAM.CAP_SCREENSHOT_PATH";
static const std::string CAP_PAUSED_AT_FRAME = "CAPTUREPARAM.CAP_PAUSED_AT_FRAME";

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

