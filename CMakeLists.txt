cmake_minimum_required(VERSION 2.6)

cmake_policy (SET CMP0020 NEW)

project(BioTracker)

# project dependecies
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using Clang or GCC
	# minimal optimization and debug symbols for debug builds
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
	# enable optimization and warnings-are-errors for release builds
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DQT_NO_DEBUG -Werror -pedantic -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor -Wsign-promo")
	# enable C++1y
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
        # enable all warnings
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
    # Force to always compile with W3
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    endif()
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Qt options
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(
	${PROJECT_SOURCE_DIR}
	SYSTEM ${Qt5Widgets_INCLUDE_DIRS}
	SYSTEM ${OpenCV_INCLUDE_DIRS}
	SYSTEM ${Qt5OpenGL_INCLUDE_DIRS}
	SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/
        SYSTEM ${PROJECT_SOURCE_DIR}/external
)

add_subdirectory(source)

install(FILES "${CMAKE_SOURCE_DIR}/config.ini"
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/source/)

