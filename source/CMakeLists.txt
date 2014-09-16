set(src main.cpp)

file(GLOB gui_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	gui/*.cpp gui/*.h)
source_group(gui FILES ${gui_src})

file(GLOB helper_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	helper/*.cpp helper/*.h)
source_group(helper FILES ${helper_src})

file(GLOB settings_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	settings/*.cpp settings/*.h)
source_group(settings FILES ${settings_src})

file(GLOB trackedObject_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	trackedObject/*.cpp trackedObject/*.h)
source_group(trackedObject FILES ${trackedObject_src})

file(GLOB tracking_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	tracking/*.cpp tracking/*.h)
source_group(tracking FILES ${tracking_src})

file(GLOB video_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	video/*.cpp video/*.h)
source_group(video FILES ${video_src})

file(GLOB_RECURSE ui 
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.ui")
file(GLOB_RECURSE qrc 
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.qrc")

qt5_wrap_ui(UI_HEADERS ${ui})
qt5_add_resources(UI_RESOURCES ${qrc})

add_subdirectory(tracking)

add_executable(BioTracker 
	${src} 
	${gui_src} 
	${helper_src}
	${settings_src}
	${trackedObject_src}
	${tracking_src}
	${video_src}
	${UI_HEADERS}
	${UI_RESOURCES})
 
target_link_libraries(BioTracker
	BeesBookTagMatcher
	ColorPatchTracker
	FishParticle
	SampleTracker
	SimpleTracker
	LandmarkTracker
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
)