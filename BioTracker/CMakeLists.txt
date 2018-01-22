cmake_minimum_required(VERSION 2.6)
#(Minimal benötigte CMake-Version wenn z.B. bestimmte CMake-Kommandos benutzt werden)

cmake_policy (SET CMP0020 NEW)
#cmake_policy(SET CMP0054 NEW)

# Der Projektname
project(Biotracker)
set(CMAKE_CXX_FLAGS "-std=c++11" CACHE STRING "compile flags" FORCE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

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

if(CMAKE_SYSTEM MATCHES "Windows")
	set(Boost_USE_STATIC_LIBS   ON)
	set(Boost_USE_MULTITHREADED ON)
endif()
find_package(Boost REQUIRED system filesystem chrono thread timer program_options)


exec_program(
    "git"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS "log --pretty=format:%h -n 1"
    OUTPUT_VARIABLE VERSION )
add_definitions( -DGIT_HASH="${VERSION}" )
message(STATUS "Configured git hash=${VERSION}")
message(STATUS "Configured QT version=${Qt5Core_VERSION}")
message(STATUS "Configured QT version=${Qt5Core_VERSION}")
add_definitions( -DMyQT_VERSION="${Qt5Core_VERSION}" )
add_definitions( -DMyCV_VERSION="${OpenCV_VERSION}" )
add_definitions( -DMyBT_VERSION="${Boost_LIB_VERSION}" )


add_subdirectory(Interfaces/BioTrackerInterfaces)
add_subdirectory(Plugin/BackgroundSubtraction)
add_subdirectory(Plugin/LukasKanade)
add_subdirectory(Utils/BioTrackerUtils)
add_subdirectory(CoreApp/BioTracker)
#add_subdirectory(Plugin/FlyHeart)



