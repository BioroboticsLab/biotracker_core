file(GLOB src
	*.cpp *.h)

add_library(SampleTracker STATIC
	${src})