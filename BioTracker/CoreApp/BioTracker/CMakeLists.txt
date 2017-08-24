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

set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

include_directories(${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

message("Configuring Biotracker_core..." )
set(EXE_NAME Biotracker_core)
file(GLOB_RECURSE Biotracker_core RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp *.ui)
add_executable(${EXE_NAME} ${Biotracker_core} guiresources.qrc)
target_link_libraries(${EXE_NAME} ${LIBS})
add_dependencies(${EXE_NAME} Biotracker_interfaces Biotracker_utility BackgroundSubtraction.tracker)



