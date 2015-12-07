# BioTracker
The BioTracker is an open-source video tracking framework for the analysis of animal behavior.
It allows users to apply various tracking algorithms on images, videos and camera streams.
This repository contains the core functionality of the BioTracker application that can be embedded in other applications.
Please consult the [documentation wiki](https://github.com/BioroboticsLab/biotracker_core/wiki/Documentation) for an complete API documentation.
To get insight into the technology and patterns used you can hold on to the [development wiki](https://github.com/BioroboticsLab/biotracker_core/wiki/Development)

This core library is used in the [BioTracker GUI](https://github.com/BioroboticsLab/biotracker_gui):
![BioTracker GUI](https://cloud.githubusercontent.com/assets/831215/10982001/2ca550c6-840b-11e5-9bd8-f4e13846438d.png) 

## Dependencies
### Unix
You can find an up-to-date list of [dependencies](https://github.com/BioroboticsLab/DockerFiles/blob/master/ubuntu-15.10-biotracker/dependencies.sh)
needed to build the BioTracker in our docker repository.

### OSX
tbd

### Windows
tbd

## Setup
The easiest way to use BioTracker Core is with [cmake](https://cmake.org/) in combination with [cpm](http://www.cpm.rocks/).
You should first add our [cmakeconfig](https://github.com/BioroboticsLab/cmakeconfig) to you CMakeList.txt:
```CMake
set(CPM_MODULE_NAME YourProjectName)
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

if ((DEFINED BIOROBOTICS_CMAKE_CONFIG_DIR))
    include(BIOROBOTICS_CMAKE_CONFIG_DIR)
else()
    set(BIOROBOTICS_CMAKE_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/cmakeconfig" CACHE TYPE STRING)
    find_package(Git)
    if(NOT GIT_FOUND)
      message(FATAL_ERROR "CPM requires Git.")
    endif()
    if (NOT EXISTS ${BIOROBOTICS_CMAKE_CONFIG_DIR}/CMakeLists.txt)
      message(STATUS "Cloning repo (https://github.com/BioroboticsLab/cmakeconfig.git)")
      execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/BioroboticsLab/cmakeconfig.git
        ${BIOROBOTICS_CMAKE_CONFIG_DIR}
        RESULT_VARIABLE error_code
        OUTPUT_QUIET ERROR_QUIET)
      if(error_code)
          message(FATAL_ERROR "CMAKECONFIG failed to get the hash for HEAD")
      endif()
    endif()
    include(${BIOROBOTICS_CMAKE_CONFIG_DIR}/CMakeLists.txt)
endif()
```

After that, you can simply include BioTracker core:
In your CMakeLists.txt, add:
```CMake
include_biotracker_core("master")
```
