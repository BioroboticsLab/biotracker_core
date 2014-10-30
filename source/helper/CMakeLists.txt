set(src
    SystemConfigCheck.cpp
    cv/Rectification.cpp)
    
set(hdr
    SystemConfigCheck.h
    cv/Rectification.h)

add_library(Helper STATIC
    ${src} ${hdr}
)