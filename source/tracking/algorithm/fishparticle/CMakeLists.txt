file(GLOB src
	*.cpp *.h)

file(GLOB particlefilter_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	particlefilter/*.cpp particlefilter/*.h)
source_group(particlefilter FILES ${particlefilter_src})

file(GLOB preprocessing_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	preprocessing/*.cpp preprocessing/*.h)
source_group(preprocessing FILES ${preprocessing_src})

add_library(FishParticle STATIC
	${src} 
	${particlefilter_src}
	${preprocessing_src})