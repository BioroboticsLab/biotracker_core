file(GLOB src
	*.cpp *.h)

file(GLOB cv_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	cv/*.cpp cv/*.h)

source_group(cv FILES ${cv_src})

add_subdirectory(cv)