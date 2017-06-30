cmake_minimum_required(VERSION 2.6)
#(Minimal benötigte CMake-Version wenn z.B. bestimmte CMake-Kommandos benutzt werden)

cmake_policy (SET CMP0020 NEW)
#cmake_policy(SET CMP0054 NEW)

# Der Projektname
project(Biotracker)
set(CMAKE_CXX_FLAGS "-std=c++11" CACHE STRING "compile flags" FORCE)

#Armin Berres — 11/22/2008, 3:12:41 PM (Stackoverflow)
#ZERO_CHECK will rerun cmake. You can/should execute this after changing something on your CMake files.
#Hauke - 17.05.2017
#It seems that doing the regeneration from visual studio usually leads to stale QT moc files. 
#So if you change the cmake files, better wipe the entire configuration and re-run from cmake-gui. 
#Clean build takes a while but will safe you a lot of pain.
set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Building for x64
IF(CMAKE_CL_64)
	find_package(Qt5 REQUIRED Core Gui Xml Network Widgets OpenGL Multimedia MultimediaWidgets PATHS $ENV{QT_DIR_CMAKE64})
	find_package(OpenCV REQUIRED PATHS $ENV{CV_DIR_CMAKE64})
	set (QT_DIR_CMAKE $ENV{QT_DIR_CMAKE64})
ELSE()
#Building for x86
	find_package(Qt5 REQUIRED Core Gui Xml Network Widgets OpenGL Multimedia MultimediaWidgets PATHS $ENV{QT_DIR_CMAKE32})
	find_package(OpenCV REQUIRED PATHS $ENV{CV_DIR_CMAKE32})
	set (QT_DIR_CMAKE $ENV{QT_DIR_CMAKE32})
ENDIF()


set(HMNVLibDir "Not Found" CACHE FILEPATH "")
IF("${HMNVLibDir}" MATCHES "Not Found")
ELSE()
	add_definitions(-DWITH_CUDA)
ENDif()

IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	find_package(Boost REQUIRED system filesystem chrono thread timer)
ELSE()
	#Find_package doesn't seem to work on windows, even when providing a PATHS hint...
	set(Boost_LIB_DIR $ENV{Boost_LIB_DIR} CACHE FILEPATH "")
	set(Boost_INCLUDE_DIR $ENV{Boost_INCLUDE_DIR} CACHE FILEPATH "")
	set(Boost_VERSION_STRING_DEB "-vc140-mt-gd-1_64" CACHE STRING "")
	set(Boost_VERSION_STRING_REL "-vc140-mt-1_64" CACHE STRING "")
	
	set(Boost_LIBRARIES
		debug ${Boost_LIB_DIR}/libboost_filesystem${Boost_VERSION_STRING_DEB}.lib
		debug ${Boost_LIB_DIR}/libboost_system${Boost_VERSION_STRING_DEB}.lib
		debug ${Boost_LIB_DIR}/libboost_chrono${Boost_VERSION_STRING_DEB}.lib
		debug ${Boost_LIB_DIR}/libboost_thread${Boost_VERSION_STRING_DEB}.lib
		debug ${Boost_LIB_DIR}/libboost_timer${Boost_VERSION_STRING_DEB}.lib
		debug ${Boost_LIB_DIR}/libboost_exception${Boost_VERSION_STRING_DEB}.lib
		optimized ${Boost_LIB_DIR}/libboost_filesystem${Boost_VERSION_STRING_REL}.lib
		optimized ${Boost_LIB_DIR}/libboost_system${Boost_VERSION_STRING_REL}.lib
		optimized ${Boost_LIB_DIR}/libboost_chrono${Boost_VERSION_STRING_REL}.lib
		optimized ${Boost_LIB_DIR}/libboost_thread${Boost_VERSION_STRING_REL}.lib
		optimized ${Boost_LIB_DIR}/libboost_timer${Boost_VERSION_STRING_REL}.lib
		optimized ${Boost_LIB_DIR}/libboost_exception${Boost_VERSION_STRING_REL}.lib)
	set(Boost_INCLUDE_DIRS
		${Boost_INCLUDE_DIR})
ENDIF()




add_subdirectory(CoreApp/BioTracker)
add_subdirectory(Interfaces/BioTrackerInterfaces)
add_subdirectory(Plugin/BackgroundSubtraction)
add_subdirectory(Utils/BioTrackerUtils)
add_subdirectory(Plugin/Sample)
add_subdirectory(Plugin/LukasKanade)



