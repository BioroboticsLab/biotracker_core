file(GLOB src
	*.cpp *.h)

add_library(ColorPatchTracker STATIC
	${src})