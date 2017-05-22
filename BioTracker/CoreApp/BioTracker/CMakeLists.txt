##############################################################
#### Biotracker: Core
##############################################################

set(INCLUDE_DIRS
	${INCLUDE_DIRS}
	${CMAKE_BINARY_DIR}/Interfaces/BioTrackerInterfaces/
	${CMAKE_BINARY_DIR}/Utils/BioTrackerUtils
	E:/Software/Libraries/boost_1_64_0_VS14.1
	${OpenCV_INCLUDE_DIRS} 
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5Xml_INCLUDE_DIRS}
	${Qt5Network_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Multimedia_INCLUDE_DIRS}
	${Qt5MultimediaWidgets_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
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
    Biotracker_plugin
    ${OpenCV_LIBRARIES}
    ${QTLIBS} 
	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_filesystem-vc140-mt-gd-1_64.lib
	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_system-vc140-mt-gd-1_64.lib
	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_chrono-vc140-mt-gd-1_64.lib
	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_thread-vc140-mt-gd-1_64.lib
	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_timer-vc140-mt-gd-1_64.lib
	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_exception-vc140-mt-gd-1_64.lib
	)

set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

include_directories(${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

message("Configuring Biotracker_core...")
set(EXE_NAME Biotracker_core)
file(GLOB_RECURSE Biotracker_core RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp *.ui)
add_executable(${EXE_NAME} ${Biotracker_core} )
target_link_libraries(${EXE_NAME} ${LIBS})
add_dependencies(${EXE_NAME} Biotracker_interfaces Biotracker_utility Biotracker_plugin)



