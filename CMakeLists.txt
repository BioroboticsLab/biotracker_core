cmake_minimum_required(VERSION 2.8.7)

if (POLICY CMP0020)
	cmake_policy (SET CMP0020 NEW)
endif(POLICY CMP0020)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
project(biotracker_core)

#-----------------------------------------------------------------------
# CPM configuration
#-----------------------------------------------------------------------
set(CPM_MODULE_NAME BioTrackerLib)
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

if ((DEFINED CPM_DIR) AND (DEFINED CPM_UNIQUE_ID) AND (DEFINED CPM_TARGET_NAME))
  set(CPM_LIB_TARGET_NAME ${CPM_TARGET_NAME})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CPM_DIR})
  include(CPM)
else()
  set(CPM_DIR "${CMAKE_CURRENT_BINARY_DIR}/cpm-packages" CACHE TYPE STRING)
  find_package(Git)
  if(NOT GIT_FOUND)
    message(FATAL_ERROR "CPM requires Git.")
  endif()
  if (NOT EXISTS ${CPM_DIR}/CPM.cmake)
    message(STATUS "Cloning repo (https://github.com/iauns/cpm)")
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" clone https://github.com/iauns/cpm ${CPM_DIR}
      RESULT_VARIABLE error_code
      OUTPUT_QUIET ERROR_QUIET)
    if(error_code)
      message(FATAL_ERROR "CPM failed to get the hash for HEAD")
    endif()
  endif()
  include(${CPM_DIR}/CPM.cmake)
endif()

CPM_AddModule("cpm_cereal"
    GIT_REPOSITORY https://github.com/BioroboticsLab/cpm_cereal.git
    GIT_TAG 874092ae503eec016192ddd6ff17ae23839085be
    EXPORT_MODULE true
)

if(NOT DEFINED CMAKECONFIG_PATH)
    CPM_AddModule("cmakeconfig"
        GIT_REPOSITORY "https://github.com/BioroboticsLab/cmakeconfig.git"
        GIT_TAG "master")
else()
    CPM_AddModule("cmakeconfig"
        SOURCE_DIR "${CMAKECONFIG_PATH}")
endif()

CPM_InitModule(${CPM_MODULE_NAME})

biorobotics_config()
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

# add git hooks
file(GLOB HOOK_SCRIPTS githooks/*)
if (HOOK_SCRIPTS)
    file(COPY ${HOOK_SCRIPTS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/.git/hooks)
endif (HOOK_SCRIPTS)

# project dependecies
set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Threads REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS filesystem serialization system REQUIRED)
find_package(PythonLibs REQUIRED)

# FIND ZMQ
if (ZMQ_LIBS AND ZMQ_INCLUDES)
    set(ZMQ_FOUND TRUE)
else (ZMQ_LIB AND ZMQ_INCLUDE)
    find_path(ZMQ_INCLUDE NAMES zmq.h PATHS /usr/include /usr/local/include)
    find_library(ZMQ_LIB NAMES zmq PATHS /usr/lib /usr/local/lib)

    set (ZMQ_INCLUDES ${ZMQ_INCLUDE})

    if (ZMQ_LIB)
        set (ZMQ_LIBS ${ZMQ_LIBS} ${ZMQ_LIB})
    endif (ZMQ_LIB)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(ZMQ DEFAULT_MSG ZMQ_LIBS ZMQ_INCLUDES)

endif (ZMQ_LIBS AND ZMQ_INCLUDES)

# FIND ZMQ
if (ZMQ_LIBS AND ZMQ_INCLUDES)
    set(ZMQ_FOUND TRUE)
else (ZMQ_LIB AND ZMQ_INCLUDE)
    find_path(ZMQ_INCLUDE NAMES zmq.h PATHS /usr/include /usr/local/include)
    find_library(ZMQ_LIB NAMES zmq PATHS /usr/lib /usr/local/lib)

    set (ZMQ_INCLUDES ${ZMQ_INCLUDE})

    if (ZMQ_LIB)
        set (ZMQ_LIBS ${ZMQ_LIBS} ${ZMQ_LIB})
    endif (ZMQ_LIB)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(ZMQ DEFAULT_MSG ZMQ_LIBS ZMQ_INCLUDES)

endif (ZMQ_LIBS AND ZMQ_INCLUDES)


# Qt options
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(-DQT_NO_KEYWORDS)
CPM_ExportAdditionalDefinition(-DQT_NO_KEYWORDS)

# include external dependecies as SYSTEM headers to prevent warnings from
# external files. unfortunately, this has no effect when using MSVC.
include_directories(
    ${PROJECT_SOURCE_DIR}
    SYSTEM ${Qt5Widgets_INCLUDE_DIRS}
    SYSTEM ${OpenCV_INCLUDE_DIRS}
    SYSTEM ${Qt5OpenGL_INCLUDE_DIRS}
    SYSTEM ${PYTHON_INCLUDE_DIRS}
    SYSTEM ${ZMQ_INCLUDES}
    SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/
)

add_subdirectory(biotracker)
add_subdirectory(doc)

option(BIOTRACKER_BUILD_TESTS "BIOTRACKER_BUILD_TESTS" On)
if (BIOTRACKER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test EXCLUDE_FROM_ALL)
endif()
