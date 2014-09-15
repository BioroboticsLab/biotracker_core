file(GLOB_RECURSE src "*.cpp")
file(GLOB_RECURSE hdr "*.h" "*.hpp")
file(GLOB_RECURSE ui "*.ui")

qt5_wrap_ui(UI_HEADERS ${ui})

message(${ui})
message(${UI_HEADERS})
 
add_executable(BeeTracker ${src} ${UI_HEADERS})
 
target_link_libraries(BeeTracker
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
)
