##############################################################
#### Biotracker: Utility
##############################################################

# Visual studio out-of-source friendly source groups
include(CMakeCommon/CMakeVSSugar.txt)

set(util_cmakes "${CMAKE_CURRENT_SOURCE_DIR}/CMakeCommon/" CACHE INTERNAL "util_cmakes")

message("Configuring Biotracker_utility...")
set(EXE_NAME Biotracker_utility)
add_library(${EXE_NAME} STATIC ${_util_source_list} )
target_link_libraries(${EXE_NAME} Biotracker_interfaces)
target_link_libraries(${EXE_NAME} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(${EXE_NAME} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Network Qt5::Widgets)
include_directories(${Boost_INCLUDE_DIRS})
target_include_directories(${EXE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${EXE_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${EXE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)