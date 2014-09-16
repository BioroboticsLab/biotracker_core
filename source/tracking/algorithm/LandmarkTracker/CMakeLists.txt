file(GLOB src
	*.cpp *.h)

add_library(LandmarkTracker STATIC
	${src})