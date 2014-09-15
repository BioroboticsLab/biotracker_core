cmake_minimum_required(VERSION 2.6)

project(BioTracker)

# project dependecies
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# minimal optimization and debug symbols for debug builds
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
# enable optimization and warnings-are-errors for release builds
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DQT_NO_DEBUG -Werror")

# enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Qt options
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(
  ${PROJECT_SOURCE_DIR}
  SYSTEM ${Qt5Widgets_INCLUDE_DIRS}
  SYSTEM ${OpenCV_INCLUDE_DIRS}
  SYSTEM ${Qt5OpenGL_INCLUDE_DIRS}
)

add_subdirectory(source)