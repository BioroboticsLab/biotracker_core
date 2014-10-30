set(src main.cpp)

add_subdirectory(gui)
add_subdirectory(helper)
add_subdirectory(settings)
add_subdirectory(tracking)
add_subdirectory(video)

file(GLOB_RECURSE qrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.qrc")
qt5_add_resources(UI_RESOURCES ${qrc})

add_executable(BioTracker 
    ${src} ${UI_RESOURCES}
)
 
target_link_libraries(BioTracker
    Gui
    Helper
    Settings
    Tracking
    Video
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
)
