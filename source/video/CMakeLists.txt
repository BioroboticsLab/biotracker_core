set(src
    VideoView.cpp)
    
set(hdr
    VideoView.h)

add_library(Video STATIC
    ${src} ${hdr}
)
