file(GLOB src
	*.cpp *.h)

file(GLOB_RECURSE ui
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.ui")
file(GLOB_RECURSE qrc
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.qrc")

qt5_wrap_ui(UI_HEADERS ${ui})
qt5_add_resources(UI_RESOURCES ${qrc})

add_library(LandmarkTracker STATIC
        ${src} ${ui} ${qrc})
