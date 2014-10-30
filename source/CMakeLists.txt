set(src main.cpp)

add_subdirectory(gui)
add_subdirectory(helper)
add_subdirectory(settings)
add_subdirectory(tracking)
add_subdirectory(video)

add_executable(BioTracker 
    ${src} 
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
