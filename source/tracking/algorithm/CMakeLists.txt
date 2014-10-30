add_subdirectory(BeesBookTagMatcher)
add_subdirectory(colorPatchTracker)
add_subdirectory(fishparticle)
add_subdirectory(LandmarkTracker)
add_subdirectory(SampleTracker)
add_subdirectory(simpletracker)

set(hdr
    algorithms.h)

add_library(Algorithms STATIC ${hdr})

target_link_libraries(Algorithms
    SimpleTracker
    SampleTracker
    LandmarkTracker
    BeesBookTagMatcher
    ColorPatchTracker
    FishParticleTracker
)
