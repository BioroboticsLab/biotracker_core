file(GLOB src
	*.cpp *.h)

file(GLOB resources_src
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	resources/*.cpp resources/*.h)
source_group(resources FILES ${resources_src})

add_library(BeesBookTagMatcher STATIC
	${src} 
	${resources_src})