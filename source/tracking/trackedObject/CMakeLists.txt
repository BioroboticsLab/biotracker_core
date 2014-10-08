file(GLOB src
	*.cpp *.h)

add_library(TrackedObject STATIC
	${src})