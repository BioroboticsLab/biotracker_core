
namespace CAPTUREPARAM
{
	// Video handler
	const std::string CAP_TYPE					= "CAPTUREPARAM/CAP_TYPE";
	const std::string CAP_VIDEO_FILE			= "CAPTUREPARAM/CAP_VIDEO_FILE";
	const std::string CAP_CAM_ID				= "CAPTUREPARAM/CAP_CAM_ID";
	const std::string CAP_FRAME_WIDTH			= "CAPTUREPARAM/CAP_FRAME_WIDTH";
	const std::string CAP_FRAME_HEIGHT			= "CAPTUREPARAM/CAP_FRAME_HEIGHT";	
	const std::string CAP_CAM_FPS				= "CAPTUREPARAM/CAP_CAM_FPS";
	const std::string CAP_VID_FPS				= "CAPTUREPARAM/CAP_VID_FPS";
	const std::string DELAY_TIME				= "CAPTUREPARAM/DELAY_TIME";

	// Video paths
	const std::string ENABLE_RECORD_RAW			= "CAPTUREPARAM/ENABLE_RECORD_RAW";
	const std::string ENABLE_RECORD_TRACKING	= "CAPTUREPARAM/ENABLE_RECORD_TRACKING";
	const std::string ENABLE_RECORD_TRACK_INFO	= "CAPTUREPARAM/ENABLE_RECORD_TRACK_INFO";
	
	const std::string RECORDING_TRACKING_PATH	= "CAPTUREPARAM/RECORDING_TRACKING_PATH";
	const std::string RECORDING_RAW_PATH		= "CAPTUREPARAM/RECORDING_RAW_PATH";
	const std::string TRACKING_INFO_OUTPUT_PATH	= "CAPTUREPARAM/TRACKING_INFO_OUTPUT_PATH";

	enum CAP_TYPE_ENUM
	{
		CAP_FROM_CAM = 0,
		CAP_FROM_FILE = 1
	};

	enum CAP_STATE_ENUM
	{
		CAP_FINISHED = 0,
		CAP_PAUSE = 1,
		CAP_AVAILABLE = 2,
		CAP_EXITED = 3
	};

	enum TRACKER_METHOD_ENUM
	{
		NN2d = 0,
		PFilter = 1
	};

}

namespace TRACKERPARAM
{
	// Parameter for the opencv BackgroundSubtractorMOG2 class 
	const std::string BG_MOG2_HISTORY			= "TRACKERPARAM/BG_MOG2_HISTORY";
	const std::string BG_MOG2_VAR_THRESHOLD		= "TRACKERPARAM/BG_MOG2_VAR_THRESHOLD";
	const std::string BG_MOG2_SHADOW_DETECTION	= "TRACKERPARAM/BG_MOG2_SHADOW_DETECTION";
	const std::string BG_MOG2_BACKGROUND_RATIO	= "TRACKERPARAM/BG_MOG2_BACKGROUND_RATIO";

	// Parameters for enabling methods
	const std::string ENABLE_BG_SUBTRACTION		= "TRACKERPARAM/ENABLE_BG_SUBTRACTION";
	const std::string ENABLE_BG_IMAGE			= "TRACKERPARAM/ENABLE_BG_IMAGE";
	const std::string ENABLE_BINARIZING			= "TRACKERPARAM/ENABLE_BINARIZING";
	const std::string ENABLE_ERODE				= "TRACKERPARAM/ENABLE_ERODE";
	const std::string ENABLE_DILATE				= "TRACKERPARAM/ENABLE_DILATE";
	const std::string IMAGE_VIEW_TYPE			= "TRACKERPARAM/IMAGE_VIEW_TYPE";

	// Mapping issue
	const std::string MAX_MAPPED_DISTANCE_PX	= "TRACKERPARAM/MAX_MAPPED_DISTANCE_PX";
	const std::string MAX_MAPPED_DISTANCE_CM	= "TRACKERPARAM/MAX_MAPPED_DISTANCE_CM";	

	// Blob dectection issue
	const std::string MAX_BLOB_SIZE				= "TRACKERPARAM/MAX_BLOB_SIZE";
	const std::string MIN_BLOB_SIZE				= "TRACKERPARAM/MIN_BLOB_SIZE";

	// Parameters for image pre-processing step
	const std::string SIZE_ERODE				= "TRACKERPARAM/SIZE_ERODE";
	const std::string SIZE_DILATE				= "TRACKERPARAM/SIZE_DILATE";
	const std::string THRESHOLD_BINARIZING		= "TRACKERPARAM/THRESHOLD_BINARIZING";

	const std::string TRACKER_ENABLED			= "TRACKERPARAM/TRACKER_ENABLED";
	const std::string TRACKER_METHOD			= "TRACKERPARAM/TRACKER_METHOD";

	// Image processing type
	enum IMAGE_DISPLAY_TYPE {
		IMAGE_OR = 0,
		IMAGE_BI = 1,
		IMAGE_ER = 2,
		IMAGE_DI = 3,
		IMAGE_FG = 4,
		IMAGE_BG = 5,
		IMAGE_PP = 6,
	};
}

namespace CONFIGPARAM
{
	// System config
	const std::string CONFIG_INI_FILE			= "./config.ini";
}