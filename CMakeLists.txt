﻿cmake_minimum_required(VERSION 3.15)

exec_program("git" ${CMAKE_CURRENT_SOURCE_DIR} ARGS "describe --dirty --always" OUTPUT_VARIABLE SOURCE_VERSION)

project(BioTracker
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(HMNVLibDir "Not Found" CACHE FILEPATH "")

if( CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR )
    find_package(biotracker-utility REQUIRED)
endif()

add_subdirectory(Src)
