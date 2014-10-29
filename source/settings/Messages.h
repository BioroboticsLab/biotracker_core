#ifndef Messages_h
#define Messages_h
#include <iostream>

#include "source/settings/ParamNames.h"

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
	}

	namespace SYSTEM
	{
		const std::string APPLICATION_CANNOT_START					= "Application cannot start, because ...";
		const std::string NO_OPENGL									= "This system has no OpenGL support!";
		const std::string MISSING_CONFIGURATION_FILE				= std::string("Configuration file (\"").append(CONFIGPARAM::CONFIGURATION_FILE).append("\") is missing!");
		const std::string CONFIGURATION_FILE_CREATED				= "Configuration file has been created!";
		const std::string CONFIGURATION_FILE_CANNOT_BE_CREATED		= "Configuration file cannot be created!";		
	}

	enum MTYPE
	{
		NOTIFICATION,
		WARNING,
		FAIL
	};
}  
#endif // !Messages_h

