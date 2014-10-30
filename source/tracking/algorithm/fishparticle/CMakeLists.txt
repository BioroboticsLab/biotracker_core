set(src
    ParticleFishTracker.cpp
    particlefilter/GridParticleBuckets.cpp
    particlefilter/Particle.cpp
    particlefilter/ParticleBrightnessObserver.cpp
    particlefilter/ParticleClusters.cpp
    preprocessing/FishBackgroundSubtractor.cpp
    preprocessing/FramePreprocessor.cpp)
    
set(hdr
    ParticleFishTracker.h
    particlefilter/GridParticleBuckets.h
    particlefilter/Particle.h
    particlefilter/ParticleBrightnessObserver.h
    particlefilter/ParticleClusters.h
    preprocessing/FishBackgroundSubtractor.h
    preprocessing/FramePreprocessor.h)

add_library(FishParticleTracker STATIC
    ${src} ${hdr}
)