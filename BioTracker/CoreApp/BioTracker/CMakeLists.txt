##############################################################
#### Biotracker: Core
##############################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
	

# Visual studio out-of-source friendly source groups
include(${util_cmakes}CMakeVSSugar.txt)

message("Configuring Biotracker_core..." )
set(EXE_NAME Biotracker_core)
add_executable(${EXE_NAME} ${_util_source_list} guiresources.qrc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries (${EXE_NAME} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Network Qt5::Widgets)
target_link_libraries (${EXE_NAME} Qt5::Multimedia Qt5::MultimediaWidgets Qt5::OpenGL )
target_link_libraries(${EXE_NAME} Biotracker_interfaces Biotracker_utility)
target_link_libraries(${EXE_NAME} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})
IF("${HMNVLibDir}" MATCHES "Not Found")
ELSE()
	target_link_libraries(${EXE_NAME} ${HMNVLibDir}/lib/NvEncInterace.lib)
	target_compile_definitions(${EXE_NAME} PRIVATE WITH_CUDA=1)
ENDIF()

set_target_properties(${EXE_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${EXE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)