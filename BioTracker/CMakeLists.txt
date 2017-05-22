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

find_package(Qt5 REQUIRED Core Gui Xml Network Widgets Opengl Multimedia MultimediaWidgets PATHS $ENV{QT_DIR_CMAKE})
find_package(OpenCV REQUIRED PATHS $ENV{CV_DIR_CMAKE})

#Building for x64
IF(CMAKE_CL_64)
	find_package(Qt5 REQUIRED Core Gui Xml Network Widgets PATHS $ENV{QT_DIR_CMAKE64})
	find_package(OpenCV REQUIRED PATHS $ENV{CV_DIR_CMAKE64})
	set (QT_DIR_CMAKE $ENV{QT_DIR_CMAKE64})
ELSE()
#Building for x86
	find_package(Qt5 REQUIRED Core Gui Xml Network Widgets PATHS $ENV{QT_DIR_CMAKE32})
	find_package(OpenCV REQUIRED PATHS $ENV{CV_DIR_CMAKE32})
	set (QT_DIR_CMAKE $ENV{QT_DIR_CMAKE32})
ENDIF()

# Somehow this is required on Windows. TODO: if linux...
set(_qt5_module_paths ${_qt5_module_paths}\\..\\..\\)
set(_qt5_root_dir ${_qt5_root_dir}\\..\\..\\)
set(_qt5_install_prefix ${_qt5_install_prefix}\\..\\..\\)
 

add_subdirectory(CoreApp/BioTracker)
add_subdirectory(Interfaces/BioTrackerInterfaces)
add_subdirectory(Plugin/BioTrackerPlugin)
add_subdirectory(Utils/BioTrackerUtils)



