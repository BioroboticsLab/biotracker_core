##############################################################
#### BioTracker Core
##############################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
if( CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR )
    include(BT_Utility/VSSugar)
else()
    include(${BT_Utility_SOURCE_DIR}/cmake/VSSugar.cmake)
endif()
VSSourceGroups(${CMAKE_CURRENT_LIST_DIR})

add_definitions(-DBIORACKER_GIT_HASH="${GIT_HASH}")
add_definitions(-DBIOTRACKER_GIT_TAG="${GIT_TAG}")

set(TN BioTracker)
add_executable (${TN})

target_include_directories(${TN} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries (${TN} BT_Utility)
target_link_libraries (${TN} RF_behavior_loader)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Xml Network Widgets Multimedia MultimediaWidgets OpenGL)
target_link_libraries (${TN} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Network Qt5::Widgets)
target_link_libraries (${TN} Qt5::Multimedia Qt5::MultimediaWidgets Qt5::OpenGL )

find_package(Boost REQUIRED system filesystem chrono thread timer program_options)
target_link_libraries(${TN} 
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

option(WITH_PYLON5 "Support Pylon 5 cameras" OFF)
if(WITH_PYLON5)
    find_package(Pylon5)
    if(Pylon5_FOUND)
        message("Pylon5 support enabled")
        target_compile_definitions(${TN} PRIVATE HAS_PYLON=1)
        target_link_libraries(${TN} Pylon5::Base Pylon5::Utility Pylon5::GenAPI Pylon5::GCBase)
    endif()
endif()

IF("${HMNVLibDir}" MATCHES "Not Found")
ELSE()
	target_link_libraries(${TN} ${HMNVLibDir}/lib/NvEncInterace.lib)
	target_compile_definitions(${TN} PRIVATE WITH_CUDA=1)
ENDIF()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(${TN} stdc++fs)
endif()


message(STATUS "Configured CV version=${OpenCV_VERSION}")
message(STATUS "Configured QT version=${Qt5Core_VERSION}")
message(STATUS "Configured Boost version=${Boost_LIB_VERSION}")
add_definitions( -DMyQT_VERSION="${Qt5Core_VERSION}" )
add_definitions( -DMyCV_VERSION="${OpenCV_VERSION}" )
add_definitions( -DMyBT_VERSION="${Boost_LIB_VERSION}" )

include(Locate)
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    install(TARGETS ${TN} DESTINATION .)

    option(INSTALL_REQUIRED_SYSTEM_LIBRARIES "Install required system libraries" OFF)
    if(INSTALL_REQUIRED_SYSTEM_LIBRARIES)
        set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
        include(InstallRequiredSystemLibraries)
    endif()

    include(WinDeployQt5)
    deploy_qt5(${TN} DESTINATION .)
	
	#boost_program_options;boost_filesystem;bt_interfaces;
	#calib3d;videoio;imgcodecs;imgproc;core;
    set(INSTALL_CUDA_COMPONENTS "" CACHE STRING "CUDA components to install")
    locate_cuda(shared_libraries COMPONENTS ${INSTALL_CUDA_COMPONENTS})

    set(INSTALL_OPENCV_COMPONENTS "" CACHE STRING "OpenCV components to install")
    locate_opencv(shared_libraries COMPONENTS ${INSTALL_OPENCV_COMPONENTS})

    set(INSTALL_SHARED_LIBRARIES "" CACHE STRING "Additional shared libraries to install")
    locate_shared_library(shared_libraries ${INSTALL_SHARED_LIBRARIES})

    locate_shared_library(shared_libraries bt_interfaces)

    install(FILES ${shared_libraries} DESTINATION .)
endif()

set(CPACK_PACKAGE_NAME "${TN}")
set(CPACK_PACKAGE_VENDOR "Biorobotics Lab / FU Berlin")

set(CPACK_PACKAGE_VERSION ${GIT_TAG})

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    option(PACKAGE_WINDOWS_MSI "Create .msi package" OFF)
    if(PACKAGE_WINDOWS_MSI)
        set(CPACK_GENERATOR "WIX")
        set(CPACK_WIX_UPGRADE_GUID "e47473db-3d71-4c42-8a58-d3eac87aa0bd")

        set(CPACK_PACKAGE_INSTALL_DIRECTORY "RoboFish/${TN}")

        set(CPACK_WIX_PROGRAM_MENU_FOLDER "RoboFish")

        set_property(INSTALL "$<TARGET_FILE_NAME:${TN}>"
            PROPERTY CPACK_START_MENU_SHORTCUTS "${TN}"
        )

        set_property(INSTALL "$<TARGET_FILE_NAME:${TN}>"
            PROPERTY CPACK_DESKTOP_SHORTCUTS "${TN}"
        )
		
	    set(CPACK_WIX_LICENSE_RTF ${CMAKE_SOURCE_DIR}/Src/cmake/WIX/licence.rtf)
	    set(CPACK_WIX_TEMPLATE "${CMAKE_SOURCE_DIR}/Src/cmake/WIX/WIX.template.in")
    endif()
endif()

include(CPack)

target_sources(${TN}
PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/BioTracker3App.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/GuiContext.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/guiresources.qrc"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/IControllerCfg.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerAnnotations.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerAreaDescriptor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerCommands.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerCoreParameter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerDataExporter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerGraphicScene.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerMainWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerNotifications.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerPlayer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerPlugin.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerTextureObject.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/ControllerTrackedComponentCore.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Controller/null_Controller.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/IStates/IPlayerState.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/AreaDescriptor/AreaInfo.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/AreaDescriptor/AreaInfoElement.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/AreaDescriptor/AreaMemory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/AreaDescriptor/Rectification.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/DataExporters/DataExporterCSV.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/DataExporters/DataExporterGeneric.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/DataExporters/DataExporterJson.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/DataExporters/DataExporterSerialize.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/MediaPlayerStateMachine.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/PlayerStates/PStateGoToFrame.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/PlayerStates/PStateInitial.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/PlayerStates/PStateInitialStream.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/PlayerStates/PStatePause.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/PlayerStates/PStatePlay.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/PlayerStates/PStateStepBack.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/PlayerStates/PStateStepForw.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayerStateMachine/PlayerStates/PStateWait.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/UndoCommands/TrackCommands.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/Annotations.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/BioTracker3ProxyMat.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/CoreParameter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/ImageStream.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/MediaPlayer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/null_Model.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Model/TextureObject.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/util/CLIcommands.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/util/VideoCoder.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/util/Config.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/AreaDesciptor/AreaDescriptor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/AreaDesciptor/EllipseDescriptor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/AreaDesciptor/RectDescriptor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/AnnotationsView.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/CameraDevice.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/ComponentShape.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/CoreParameterView.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/GLVideoView.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/GraphicsScene.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/GraphicsView.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/MainWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/NotificationLogBrowser.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/SettingsWindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/TextureObjectView.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/TrackedComponentView.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/TrackedImageView.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/VideoControllWidget.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/Utility/RotationHandle.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/Utility/SwitchButton.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/View/Utility/Tracer.cpp"
)



