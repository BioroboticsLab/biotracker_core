##############################################################
#### Biotracker: Core
##############################################################

set(INCLUDE_DIRS
	${INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/Interfaces/BioTrackerInterfaces/
	${CMAKE_SOURCE_DIR}/Utils/BioTrackerUtils
	${CMAKE_SOURCE_DIR}/CoreApp/BioTracker/
	${PROJECT_BINARY_DIR}/CoreApp/BioTracker/
    ${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS} 
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5Xml_INCLUDE_DIRS}
	${Qt5Network_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Multimedia_INCLUDE_DIRS}
	${Qt5MultimediaWidgets_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
	${HMNVLibDir}/inc/
	)

set(QTLIBS 
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
    Qt5::Network
    Qt5::Widgets
	Qt5::Multimedia
	Qt5::MultimediaWidgets
	)
set(LIBS 
    Biotracker_interfaces
    Biotracker_utility
    ${OpenCV_LIBRARIES}
	${Boost_LIBRARIES}
    ${QTLIBS}
	)
	
IF("${HMNVLibDir}" MATCHES "Not Found")
ELSE()
set(LIBS 
	${LIBS}
	${HMNVLibDir}/lib/NvEncInterace.lib)
ENDIF()
	
# Visual studio out-of-source friendly source groups
set(_plugin_src_root_path ${CMAKE_CURRENT_SOURCE_DIR})
file(
    GLOB_RECURSE _plugin_source_list 
    LIST_DIRECTORIES false
    "${_plugin_src_root_path}/*.c*"
    "${_plugin_src_root_path}/*.h*"
    "${_plugin_src_root_path}/*.ui*"
)
foreach(_plugin_source IN ITEMS ${_plugin_source_list})
    get_filename_component(_plugin_source_path "${_plugin_source}" PATH)
    file(RELATIVE_PATH _plugin_source_path_rel "${_plugin_src_root_path}" "${_plugin_source_path}")
    string(REPLACE "/" "\\" _plugin_group_path "${_plugin_source_path_rel}")
    source_group("${_plugin_group_path}" FILES "${_plugin_source}")
endforeach()

set(CMAKE_INCLUDE_CURRENT_DIR OFF)

include_directories(${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

message("Configuring Biotracker_core..." )
set(EXE_NAME Biotracker_core)
add_executable(${EXE_NAME} ${_plugin_source_list} guiresources.qrc)
target_link_libraries(${EXE_NAME} ${LIBS})
add_dependencies(${EXE_NAME} Biotracker_interfaces Biotracker_utility BackgroundSubtraction.tracker)

# Make this target the default target for a MSVC solution.
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${EXE_NAME})

