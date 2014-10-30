set(src
    ObjectModel.cpp
    TrackedObject.cpp)
    
set(hdr
    ObjectModel.h
    TrackedObject.h)

add_library(TrackedObject STATIC
    ${src} ${hdr}
)
