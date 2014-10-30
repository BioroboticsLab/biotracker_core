set(src
    BioTracker.cpp)
    
set(hdr
    BioTracker.h)
    
set(ui
    BioTracker.ui)

set(qrc
    BioTracker.qrc)

qt5_wrap_ui(UI_HEADERS ${ui})
qt5_add_resources(UI_RESOURCES ${qrc})

add_library(Gui STATIC
    ${src} ${hdr} ${ui} ${qrc}
)
