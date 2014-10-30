set(src
    Param.cpp
    Settings.cpp)
    
set(hdr
    Messages.h
    Param.h
    ParamNames.h
    Settings.h)

add_library(Settings STATIC
    ${src} ${hdr}
)
