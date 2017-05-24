#pragma once

#include <iostream>


namespace MSGS
{	
	namespace TRACKER
	{	
		const std::string ERROR_WRITING_RAW_VIDEO_FILE				= "Error, cannot write raw video file, please check the output codec!";
		const std::string ERROR_WRITING_TRACKING_VIDEO_FILE			= "Error, cannot write tracking video file, please check the output codec!";
		const std::string ERROR_WRITING_TRACKING_INFO_FILE			= "Error, tracking output file cannot be created or opened!";
		const std::string NOTIFICATION_WRITING_TRACKING_INFO_FILE	= "Tracking output file opened or created!";
	}

	namespace GUI
	{
		const std::string RUN_CAPTURE								= "Video capture started!";
		const std::string STEP_CAPTURE								= "Video stepped ...";
		const std::string STOP_CAPTURE								= "Video capture stopped!";
		const std::string TANK_RESET								= "Fish tank reset!";
	}

	enum MTYPE
	{
		NOTIFICATION,
		WARNING,
		ERR,
	};
}
