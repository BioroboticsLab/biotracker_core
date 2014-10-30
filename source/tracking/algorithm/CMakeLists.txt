#add_subdirectory(BeesBookTagMatcher)
#add_subdirectory(colorPatchTracker)
#add_subdirectory(fishparticle)
#add_subdirectory(nn2dMappingTracker)
#add_subdirectory(LandmarkTracker)
#add_subdirectory(SampleTracker)
add_subdirectory(simpletracker)

add_library(Algorithms STATIC)

target_link_libraries(Algorithms
    SimpleTracker
)