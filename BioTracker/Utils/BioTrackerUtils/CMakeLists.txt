##############################################################
#### Biotracker: Utility
##############################################################

set(INCLUDE_DIRS
	${INCLUDE_DIRS}
#	E:/Software/Libraries/boost_1_64_0_VS14.1
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
    ${OpenCV_LIBRARIES}
    ${Boost_LIBRARIES}
    ${QTLIBS} 
#	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_filesystem-vc140-mt-gd-1_64.lib
#	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_system-vc140-mt-gd-1_64.lib
#	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_chrono-vc140-mt-gd-1_64.lib
#	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_thread-vc140-mt-gd-1_64.lib
#	E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0\libboost_timer-vc140-mt-gd-1_64.lib
	)

set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

include_directories(${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

message("Configuring Biotracker_utility...")
set(EXE_NAME Biotracker_utility)
file(GLOB_RECURSE Biotracker_utility RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp *.ui)
#add_executable(${EXE_NAME} ${Biotracker_utility} )
add_library(${EXE_NAME} STATIC ${Biotracker_utility} )
target_link_libraries(${EXE_NAME} ${LIBS})



