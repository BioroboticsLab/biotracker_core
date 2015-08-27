add_subdirectory(algorithm)
add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(test)
add_subdirectory(util)

add_library(${CPM_LIB_TARGET_NAME})

target_link_libraries(${CPM_LIB_TARGET_NAME}
    Core
    Util
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${CPM_LIBRARIES})
