set(src
    Param.cpp
    Settings.cpp
    SystemCompatibilityCheck.cpp)
    
set(hdr
    Messages.h
    Param.h
    ParamNames.h
    Settings.h
    SystemCompatibilityCheck.h)

add_library(Settings STATIC
    ${src} ${hdr}
)
