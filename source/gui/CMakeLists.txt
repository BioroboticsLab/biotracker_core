set(src
    BioTracker.cpp)
    
set(hdr
    BioTracker.h)
    
set(ui
    BioTracker.ui)

qt5_wrap_ui(UI_HEADERS ${ui})

add_library(Gui STATIC
    ${src} ${hdr} ${UI_HEADERS}
)
