set(src
    SampleObject.cpp
    SampleTracker.cpp)
    
set(hdr
    SampleObject.h
    SampleTracker.h)

add_library(SampleTracker STATIC
    ${src} ${hdr}
)
