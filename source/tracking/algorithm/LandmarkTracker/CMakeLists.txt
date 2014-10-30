set(src
    glwidget.cpp
    LandmarkTracker.cpp
    toolwindow.cpp)
    
set(hdr
    glwidget.h
    LandmarkTracker.h
    toolwindow.h)
    
set(ui
    toolwindow.ui)

qt5_wrap_ui(UI_HEADERS ${ui})

add_library(LandmarkTracker STATIC
    ${src} ${hdr} ${UI_HEADERS}
)
