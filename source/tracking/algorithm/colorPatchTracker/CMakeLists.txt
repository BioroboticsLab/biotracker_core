set(src
    colorPatchTracker.cpp)
    
set(hdr
    colorPatchTracker.h)

add_library(ColorPatchTracker STATIC
    ${src} ${hdr}
)
