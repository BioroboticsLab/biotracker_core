##############################################################
#### Biotracker: Sampletracker
##############################################################

set(INCLUDE_DIRS
	${INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/Interfaces/BioTrackerInterfaces/
	${CMAKE_SOURCE_DIR}/Utils/BioTrackerUtils
	${CMAKE_SOURCE_DIR}/Plugin/Sample
	${PROJECT_BINARY_DIR}/Plugin/Sample/
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
    Biotracker_utility
    ${OpenCV_LIBRARIES}
    ${Boost_LIBRARIES}
    ${QTLIBS} 
	)
	
	
add_definitions( -DBIOTRACKERPLUGIN_LIBRARY=1 )

set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

include_directories(${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

message("Configuring Sample...")
set(EXE_NAME Sample.tracker)
file(GLOB_RECURSE Sample RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp *.ui)
add_library(${EXE_NAME} SHARED ${Sample} )
target_link_libraries(${EXE_NAME} ${LIBS})
add_dependencies(${EXE_NAME} Biotracker_interfaces Biotracker_utility)



