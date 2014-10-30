set(src
    CvHelper.cpp
    StringHelper.cpp)
    
set(hdr
    CvHelper.h
    StringHelper.h)

add_library(Helper STATIC
    ${src} ${hdr}
)