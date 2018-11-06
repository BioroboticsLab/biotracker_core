cmake_minimum_required(VERSION 3.12)

cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0054 NEW)

exec_program("git" ${CMAKE_CURRENT_SOURCE_DIR} ARGS "log --pretty=format:%h -n 1" OUTPUT_VARIABLE GIT_HASH)
exec_program("git" ${CMAKE_CURRENT_SOURCE_DIR} ARGS "describe --tags --abbrev=0" OUTPUT_VARIABLE GIT_TAG)

project(BioTracker
    VERSION ${GIT_TAG}
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
    find_package(BT_Utility 0.1.1 REQUIRED)
    find_package(RF_behavior_loader 0.1.1 REQUIRED)
endif()

add_subdirectory(Src)



