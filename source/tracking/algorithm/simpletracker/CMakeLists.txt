file(GLOB src
	*.cpp *.h)

add_library(SimpleTracker STATIC
	${src})