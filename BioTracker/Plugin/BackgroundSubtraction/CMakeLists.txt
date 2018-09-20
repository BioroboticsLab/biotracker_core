##############################################################
#### Biotracker: BackgroundSubtraction
##############################################################
	
	
add_definitions( -DBIOTRACKERPLUGIN_LIBRARY=1 )

include(${util_cmakes}CMakeVSSugar.txt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

message("Configuring  BackgroundSubtraction...")
set(EXE_NAME  BackgroundSubtraction.tracker)
add_library(${EXE_NAME} SHARED ${_util_source_list} )

target_link_libraries(${EXE_NAME} ${LIBS})
target_link_libraries (${EXE_NAME} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Network Qt5::Widgets)
target_link_libraries (${EXE_NAME} Qt5::Multimedia Qt5::MultimediaWidgets Qt5::OpenGL )
target_link_libraries(${EXE_NAME} Biotracker_interfaces Biotracker_utility)
target_link_libraries(${EXE_NAME} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})

set_target_properties(${EXE_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${EXE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)