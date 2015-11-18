

file(GLOB_RECURSE src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE hdr RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file(GLOB_RECURSE ui RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.ui)
file(GLOB_RECURSE qrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qrc)

qt5_add_resources(UI_RESOURCES ${qrc})
qt5_wrap_ui(UI_HEADERS ${ui})

add_executable(BioTracker
    ${src} ${hdr} ${UI_RESOURCES} ${UI_HEADERS}
)
include_directories( SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(BioTracker
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${CPM_LIBRARIES}
)

## automatic copy of required dlls to install prefix for windows
#if(WIN32)
#	set(APPS "\${CMAKE_INSTALL_PREFIX}/BioTracker.exe")
#	install(TARGETS BioTracker
#		BUNDLE DESTINATION . COMPONENT Runtime
#		RUNTIME DESTINATION . COMPONENT Runtime)
#
#	set(PLUGINS "")
#	file(TO_CMAKE_PATH ${OpenCV_DIR} OpenCV_DIR)
#	set(LIBRARY_DIRS "${OpenCV_DIR} ${Qt5Core_DIR} ${Qt5Gui_DIR} ${Qt5OpenGL_DIR} ${Qt5Widgets_DIR}")
#	install(CODE "
#	  include(BundleUtilities)
#	  fixup_bundle(\"${APPS}\" \"\${PLUGINS}\" \"${LIBRARY_DIRS}\")
#	  " COMPONENT Runtime)
#	install(FILES
#	  ${Qt5Core_DIR}/../../../plugins/platforms/qwindows.dll
#	  DESTINATION "${CMAKE_INSTALL_PREFIX}/platforms"
#	)
#
#	include(GetGitRevisionDescription)
#	get_git_head_revision(GIT_REFSPEC GIT_SHA1)
#	string(SUBSTRING ${GIT_SHA1} 0 7 GIT_SHA1)
#	string(TIMESTAMP timestamp "%Y%m%d")
#
#	set(CPACK_PACKAGE_FILE_NAME "BioTracker-${timestamp}-${GIT_SHA1}")
#	set(CPACK_GENERATOR "ZIP")
#
#	include(CPACK)
#endif()
#
## add files to source groups to preserve folder structure in visual studio
## adapted from: http://goo.gl/Xde41i
#
#set(ALL_FILES ${src} ${hdr} ${ui} ${qrc})
#
#foreach(FILE ${ALL_FILES})
#  get_filename_component(PARENT_DIR "${FILE}" PATH)
#
#  # skip src or include and changes /'s to \\'s
#  string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
#  string(REPLACE "/" "\\" GROUP "${GROUP}")
#
#  # group into categories
#  if ("${FILE}" MATCHES ".*\\.cpp")
#    set(GROUP "Source Files\\${GROUP}")
#  elseif("${FILE}" MATCHES ".*\\.h")
#    set(GROUP "Header Files\\${GROUP}")
#  elseif("${FILE}" MATCHES ".*\\.ui")
#    set(GROUP "UI Files\\${GROUP}")
#  elseif("${FILE}" MATCHES ".*\\.qrc")
#    set(GROUP "Ressource Files\\${GROUP}")
#  endif()
#
#  source_group("${GROUP}" FILES "${FILE}")
#endforeach()
#
## also add generated files to a group
#
#set(GENERATED_FILES ${UI_RESOURCES} ${UI_HEADERS})
#
#foreach(FILE ${GENERATED_FILES})
#  get_filename_component(REL_FILE ${FILE} NAME ${CMAKE_CURRENT_SOURCE_DIR})
#  #message(${REL_FILE})
#  get_filename_component(PARENT_DIR "${REL_FILE}" PATH)
#  #message(${PARENT_DIR})
#
#  # skip src or include and changes /'s to \\'s
#  string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
#  string(REPLACE "/" "\\" GROUP "${GROUP}")
#
#  # group into categories
#  set(GROUP "Generated Files\\${GROUP}")
#
#  source_group("${GROUP}" FILES "${FILE}")
#endforeach()
#
#
