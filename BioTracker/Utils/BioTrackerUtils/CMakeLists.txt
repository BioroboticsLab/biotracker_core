##############################################################
#### Biotracker: Utility
##############################################################

set(INCLUDE_DIRS
	${INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/Interfaces/BioTrackerInterfaces/
	${Boost_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS} 
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5Xml_INCLUDE_DIRS}
	${Qt5Network_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Opengl_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set(QTLIBS 
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
    Qt5::Network
    Qt5::Widgets
	)
set(LIBS 
    Biotracker_interfaces
    ${OpenCV_LIBRARIES}
    ${Boost_LIBRARIES}
    ${QTLIBS} 
	)
	
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

message("Configuring Biotracker_utility...")
set(EXE_NAME Biotracker_utility)
add_library(${EXE_NAME} STATIC ${_plugin_source_list} )
target_link_libraries(${EXE_NAME} ${LIBS})



