set(src
    FishCandidate.cpp
    SimpleTracker.cpp
    TrackedFish.cpp)
    
set(hdr
    FishCandidate.h
    SimpleTracker.h
    TrackedFish.h)

add_library(SimpleTracker STATIC
    ${src} ${hdr}
)
