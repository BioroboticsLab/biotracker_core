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

set(QTLIBS 
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
    Qt5::Network
    Qt5::Widgets
	)
set(LIBS 
    ${OpenCV_LIBRARIES}
    ${QTLIBS} 
	)

set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

include_directories(${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

message("Configuring Biotracker_interfaces...")
set(EXE_NAME Biotracker_interfaces)
file(GLOB_RECURSE Biotracker_interfaces RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp *.ui)
#add_executable(${EXE_NAME} ${Biotracker_interfaces} )
add_library(${EXE_NAME} STATIC ${Biotracker_interfaces} )
target_link_libraries(${EXE_NAME} ${LIBS})



