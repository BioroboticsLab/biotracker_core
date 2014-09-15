file(GLOB_RECURSE src "*.cpp")
file(GLOB_RECURSE hdr "*.h" "*.hpp")
file(GLOB_RECURSE ui "*.ui")
file(GLOB_RECURSE qrc "*.qrc")

qt5_wrap_ui(UI_HEADERS ${ui})
qt5_add_resources(UI_RESOURCES ${qrc})

message(${ui})
message(${UI_HEADERS})
 
add_executable(BeeTracker ${src} ${UI_HEADERS} ${UI_RESOURCES})
 
target_link_libraries(BeeTracker
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
)

INSTALL(FILES "${CMAKE_SOURCE_DIR}/config.ini" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
