##############################################################
#### Biotracker: Interfaces
##############################################################
	
set(INCLUDE_DIRS
	${INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS}
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5Xml_INCLUDE_DIRS}
	${Qt5Network_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
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

message("Configuring Biotracker_interfaces...")
set(EXE_NAME Biotracker_interfaces)
add_library(${EXE_NAME} STATIC ${_plugin_source_list} )
target_include_directories(${EXE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${EXE_NAME} ${OpenCV_LIBRARIES})
target_link_libraries(${EXE_NAME} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Network Qt5::Widgets)

set_target_properties(${EXE_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${EXE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

