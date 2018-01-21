#pragma once

#include <iostream>
#include <vector>

namespace LUKASKANADE {

    namespace CONFIGPARAM
    {
        // System config
        const std::string CONFIG_INI_FILE = "./LKTrackerConfig.ini";
    }

    namespace APPLICATIONPARAM
    {
        // System config
        const std::string APP_VERSION = "APPLICATIONPARAM/APP_VERSION";
    }

    namespace TRACKERPARAM
    {
        const std::string WNDSIZE = "TRACKERPARAM/WNDSIZE";

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

}

