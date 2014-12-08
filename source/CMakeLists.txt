file(GLOB_RECURSE src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file(GLOB_RECURSE ui RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.ui)
file(GLOB_RECURSE qrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qrc)

qt5_add_resources(UI_RESOURCES ${qrc})
qt5_wrap_ui(UI_HEADERS ${ui})

add_executable(BioTracker 
    ${src} ${hdr} ${UI_RESOURCES} ${UI_HEADERS}
)
 
target_link_libraries(BioTracker
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
)

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


