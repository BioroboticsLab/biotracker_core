#pragma once

#include <iostream>
#include <vector>


namespace CONFIGPARAM
{
	// System config
	const std::string CONFIG_INI_FILE				= "./BSTrackerConfig.ini";
}

namespace APPLICATIONPARAM
{
	// System config
	const std::string APP_VERSION					= "APPLICATIONPARAM/APP_VERSION";
}

namespace TRACKERPARAM
{

	// Parameter for the opencv BackgroundSubtractorMOG2 class 
	const std::string BG_MOG2_HISTORY				= "TRACKERPARAM/BG_MOG2_HISTORY";
	const std::string BG_MOG2_VAR_THRESHOLD			= "TRACKERPARAM/BG_MOG2_VAR_THRESHOLD";
	const std::string BG_MOG2_BACKGROUND_RATIO		= "TRACKERPARAM/BG_MOG2_BACKGROUND_RATIO";

	// Blob dectection issue
	const std::string MAX_BLOB_SIZE					= "TRACKERPARAM/MAX_BLOB_SIZE";
	const std::string MIN_BLOB_SIZE					= "TRACKERPARAM/MIN_BLOB_SIZE";

	// Parameters for image pre-processing step
	const std::string SIZE_ERODE					= "TRACKERPARAM/SIZE_ERODE";
	const std::string SIZE_DILATE					= "TRACKERPARAM/SIZE_DILATE";
	const std::string THRESHOLD_BINARIZING			= "TRACKERPARAM/THRESHOLD_BINARIZING";
	//const std::string GAUSSIAN_BLUR_SIZE			= "TRACKERPARAM/GAUSSIAN_BLUR_SIZE";
}

namespace GUIPARAM
{
    // FPS label
    const std::string ENABLE_LABEL_FPS = "GUIPARAM/ENABLE_LABEL_FPS";
    // Fish id label
    const std::string ENABLE_LABEL_FISH_ID = "GUIPARAM/ENABLE_LABEL_FISH_ID";
    // Replica marker
    const std::string ENABLE_LABEL_REPLICA = "GUIPARAM/ENABLE_LABEL_REPLICA";
    // Fish position
    const std::string ENABLE_LABEL_FISH_POS = "GUIPARAM/ENABLE_LABEL_FISH_POS";
    // Fish orientation
    const std::string ENABLE_LABEL_FISH_ORI = "GUIPARAM/ENABLE_LABEL_FISH_ORI";
    // Fish history
    const std::string ENABLE_LABEL_FISH_HISTORY = "GUIPARAM/ENABLE_LABEL_FISH_HISTORY";
    // Blobs
    const std::string ENABLE_SHOW_BLOBS = "GUIPARAM/ENABLE_SHOW_BLOBS";
    // Swap fish id
    const std::string ENABLE_SWAP_FISH_ID = "GUIPARAM/ENABLE_SWAP_FISH_ID";



    // Core view of tracked components
    const std::string ENABLE_CORE_COMPONENT_VIEW = "GUIPARAM/ENABLE_CORE_COMPONENT_VIEW";
    // Move components in core view
    const std::string ENABLE_CORE_COMPONENT_MOVE = "GUIPARAM/ENABLE_CORE_COMPONENT_MOVE";
    // Remove components in core view
    const std::string ENABLE_CORE_COMPONENT_REMOVE = "GUIPARAM/ENABLE_CORE_COMPONENT_REMOVE";
    // Swap component id in core view
    const std::string ENABLE_CORE_COMPONENT_ID_SWAP = "GUIPARAM/ENABLE_CORE_COMPONENT_ID_SWAP";
    // Add component in core view
    const std::string ENABLE_CORE_COMPONENT_ADD = "GUIPARAM/ENABLE_CORE_COMPONENT_ADD";
	// Rotate component in core view
	const std::string ENABLE_CORE_COMPONENT_ROTATE = "GUIPARAM/ENABLE_CORE_COMPONENT_ROTATE";
}

namespace FISHTANKPARAM
{
    // Tank area
    const std::string FISHTANK_AREA_WIDTH = "FISHTANKPARAM/FISHTANK_AREA_WIDTH";
    const std::string FISHTANK_AREA_HEIGHT = "FISHTANKPARAM/FISHTANK_AREA_HEIGHT";
    const std::string FISHTANK_FISH_AMOUNT = "FISHTANKPARAM/FISHTANK_FISH_AMOUNT";
    const std::string FISHTANK_AREA_CORNER1 = "FISHTANKPARAM/FISHTANK_AREA_CORNER1";
    const std::string FISHTANK_AREA_CORNER2 = "FISHTANKPARAM/FISHTANK_AREA_CORNER2";
    const std::string FISHTANK_AREA_CORNER3 = "FISHTANKPARAM/FISHTANK_AREA_CORNER3";
    const std::string FISHTANK_AREA_CORNER4 = "FISHTANKPARAM/FISHTANK_AREA_CORNER4";
    const std::string FISHTANK_ENABLE_NETWORKING = "FISHTANKPARAM/FISHTANK_ENABLE_NETWORKING";
    const std::string FISHTANK_NETWORKING_PORT = "FISHTANKPARAM/FISHTANK_NETWORKING_PORT";
}

