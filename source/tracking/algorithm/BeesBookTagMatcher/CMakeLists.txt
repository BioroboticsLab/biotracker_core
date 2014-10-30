set(src
    BeesBookTagMatcher.cpp
    resources/myGrid.cpp
    resources/myNewGrid.cpp)
    
set(hdr
    BeesBookTagMatcher.h
    resources/myGrid.h
    resources/myNewGrid.h)

add_library(BeesBookTagMatcher STATIC
    ${src} ${hdr}
)
