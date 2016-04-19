file(GLOB_RECURSE src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file(GLOB_RECURSE ui RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.ui)
file(GLOB_RECURSE qrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qrc)

qt5_add_resources(UI_RESOURCES ${qrc})
qt5_wrap_ui(UI_HEADERS ${ui})

add_library(${CPM_LIB_TARGET_NAME} SHARED ${BIOTRACKER_CORE_LIB_TYPE}
    ${src} ${hdr} ${UI_RESOURCES} ${UI_HEADERS}
)

MESSAGE( STATUS "OpenCV_LIBS: " ${OpenCV_LIBS} )
MESSAGE( STATUS "Boost_LIBRARIES: " ${Boost_LIBRARIES} )
MESSAGE( STATUS "CPM_LIBRARIES: " ${CPM_LIBRARIES} )
MESSAGE( STATUS "ZMQ_LIBRARIES: " ${ZMQ_LIBRARIES} )

target_link_libraries(${CPM_LIB_TARGET_NAME}
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${CPM_LIBRARIES}
    ${ZMQ_LIBRARIES}
)

target_include_directories(${CPM_LIB_TARGET_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # disable warning 4715 on visual studio (boost)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4715")
endif()

# add files to source groups to preserve folder structure in visual studio
# adapted from: http://goo.gl/Xde41i

set(ALL_FILES ${src} ${hdr} ${ui} ${qrc})

foreach(FILE ${ALL_FILES}) 
  get_filename_component(PARENT_DIR "${FILE}" PATH)

  # skip src or include and changes /'s to \\'s
  string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
  string(REPLACE "/" "\\" GROUP "${GROUP}")

  # group into categories
  if ("${FILE}" MATCHES ".*\\.cpp")
    set(GROUP "Source Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.h")
    set(GROUP "Header Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.ui")
    set(GROUP "UI Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.qrc")
    set(GROUP "Ressource Files\\${GROUP}")
  endif()

  source_group("${GROUP}" FILES "${FILE}")
endforeach()

# also add generated files to a group

set(GENERATED_FILES ${UI_RESOURCES} ${UI_HEADERS})

foreach(FILE ${GENERATED_FILES})
  get_filename_component(REL_FILE ${FILE} NAME ${CMAKE_CURRENT_SOURCE_DIR})
  #message(${REL_FILE})
  get_filename_component(PARENT_DIR "${REL_FILE}" PATH)
  #message(${PARENT_DIR})

  # skip src or include and changes /'s to \\'s
  string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
  string(REPLACE "/" "\\" GROUP "${GROUP}")

  # group into categories
  set(GROUP "Generated Files\\${GROUP}")

  source_group("${GROUP}" FILES "${FILE}")
endforeach()
