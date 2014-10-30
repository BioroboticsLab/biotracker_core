set(src
    TrackingAlgorithm.cpp
    TrackingThread.cpp)
    
set(hdr
    TrackingAlgorithm.h
    TrackingThread.h)

add_library(Tracking STATIC
    ${src} ${hdr}
)

add_subdirectory(algorithm)
add_subdirectory(imagePreProcessor)
add_subdirectory(trackedObject)

target_link_libraries(Tracking
    Algorithms
    ImagePreProcessor
    TrackedObject
)
