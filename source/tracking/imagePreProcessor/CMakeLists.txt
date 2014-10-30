set(src
    ImagePreProcessor.cpp)
    
set(hdr
    IImageProcessor.h
    ImagePreProcessor.h)

add_library(ImagePreProcessor STATIC
    ${src} ${hdr}
)
