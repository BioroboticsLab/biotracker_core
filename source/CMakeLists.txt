set(src 
    gui/BioTracker.cpp
    tracking/algorithm/BeesBookTagMatcher/resources/myGrid.cpp
    tracking/algorithm/BeesBookTagMatcher/resources/myNewGrid.cpp
    tracking/algorithm/BeesBookTagMatcher/BeesBookTagMatcher.cpp
    tracking/algorithm/colorPatchTracker/colorPatchTracker.cpp
    tracking/algorithm/fishparticle/preprocessing/FishBackgroundSubtractor.cpp
    tracking/algorithm/fishparticle/preprocessing/FramePreprocessor.cpp
    tracking/algorithm/fishparticle/particlefilter/Particle.cpp
    tracking/algorithm/fishparticle/particlefilter/ParticleBrightnessObserver.cpp
    tracking/algorithm/fishparticle/particlefilter/GridParticleBuckets.cpp
    tracking/algorithm/fishparticle/particlefilter/ParticleClusters.cpp
    tracking/algorithm/fishparticle/ParticleFishTracker.cpp
    tracking/algorithm/simpletracker/FishCandidate.cpp
    tracking/algorithm/simpletracker/SimpleTracker.cpp
    tracking/algorithm/simpletracker/TrackedFish.cpp
    tracking/algorithm/LandmarkTracker/glwidget.cpp
    tracking/algorithm/LandmarkTracker/toolwindow.cpp
    tracking/algorithm/LandmarkTracker/LandmarkTracker.cpp
    tracking/algorithm/SampleTracker/SampleObject.cpp
    tracking/algorithm/SampleTracker/SampleTracker.cpp
    tracking/trackedObject/ObjectModel.cpp
    tracking/trackedObject/TrackedObject.cpp
    tracking/Rectification.cpp
    tracking/imagePreProcessor/ImagePreProcessor.cpp
    tracking/TrackingAlgorithm.cpp
    tracking/TrackingThread.cpp
    settings/Param.cpp
    settings/Settings.cpp
    settings/SystemCompatibilityCheck.cpp
    video/VideoView.cpp
    helper/CvHelper.cpp
    helper/StringHelper.cpp
    main.cpp
)

set(hdr
    gui/BioTracker.h
    tracking/algorithm/BeesBookTagMatcher/resources/myGrid.h
    tracking/algorithm/BeesBookTagMatcher/resources/myNewGrid.h
    tracking/algorithm/BeesBookTagMatcher/BeesBookTagMatcher.h
    tracking/algorithm/colorPatchTracker/colorPatchTracker.h
    tracking/algorithm/fishparticle/preprocessing/FishBackgroundSubtractor.h
    tracking/algorithm/fishparticle/preprocessing/FramePreprocessor.h
    tracking/algorithm/fishparticle/particlefilter/Particle.h
    tracking/algorithm/fishparticle/particlefilter/ParticleBrightnessObserver.h
    tracking/algorithm/fishparticle/particlefilter/ParticleClusters.h
    tracking/algorithm/fishparticle/particlefilter/GridParticleBuckets.h
    tracking/algorithm/fishparticle/ParticleFishTracker.h
    tracking/algorithm/simpletracker/FishCandidate.h
    tracking/algorithm/simpletracker/SimpleTracker.h
    tracking/algorithm/simpletracker/TrackedFish.h
    tracking/algorithm/LandmarkTracker/glwidget.h
    tracking/algorithm/LandmarkTracker/LandmarkTracker.h
    tracking/algorithm/LandmarkTracker/toolwindow.h
    tracking/algorithm/SampleTracker/SampleObject.h
    tracking/algorithm/SampleTracker/SampleTracker.h
    tracking/algorithm/algorithms.h
    tracking/trackedObject/ObjectModel.h
    tracking/trackedObject/TrackedObject.h
    tracking/TrackingThread.h
    tracking/Rectification.h
    tracking/imagePreProcessor/IImageProcessor.h
    tracking/imagePreProcessor/ImagePreProcessor.h
    tracking/TrackingAlgorithm.h
    settings/Param.h
    settings/Settings.h
    settings/Messages.h
    settings/ParamNames.h
    settings/SystemCompatibilityCheck.h
    video/VideoView.h
    helper/CvHelper.h
    helper/StringHelper.h
)

set(ui
    gui/BioTracker.ui
    tracking/algorithm/LandmarkTracker/toolwindow.ui
)

set(qrc
    gui/BioTracker.qrc
)

qt5_add_resources(UI_RESOURCES ${qrc})
qt5_wrap_ui(UI_HEADERS ${ui})

add_executable(BioTracker 
    ${src} ${hdr} ${UI_RESOURCES} ${UI_HEADERS}
)
 
target_link_libraries(BioTracker
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
)

# add files to source groups to preserve folder structure in visual studio
# adapted from: http://goo.gl/Xde41i

set(ALL_FILES ${src} ${hdr} ${ui} ${qrc})

foreach(FILE ${ALL_FILES}) 
  get_filename_component(PARENT_DIR "${FILE}" PATH)

  # skip src or include and changes /'s to \\'s
  string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
  string(REPLACE "/" "\\" GROUP "${GROUP}")

  # group into categories
  if ("${FILE}" MATCHES ".*\\.cpp")
    set(GROUP "Source Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.h")
    set(GROUP "Header Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.ui")
    set(GROUP "UI Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.qrc")
    set(GROUP "Ressource Files\\${GROUP}")
  endif()

  source_group("${GROUP}" FILES "${FILE}")
endforeach()
