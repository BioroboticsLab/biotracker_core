##############################################################
#### BioTracker Core
##############################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
if( CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR )
    include(biotracker-utility/VSSugar)
else()
    include(${biotracker-utility_SOURCE_DIR}/cmake/VSSugar.cmake)
endif()
VSSourceGroups(${CMAKE_CURRENT_LIST_DIR})

add_definitions(-DBIORACKER_GIT_HASH="${GIT_HASH}")
add_definitions(-DBIOTRACKER_GIT_TAG="${GIT_TAG}")

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

set(target BioTracker)
add_executable (${target})
set_target_properties(${target} PROPERTIES OUTPUT_NAME BioTracker)

target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries (${target} biotracker-utility)
target_link_libraries (${target} robofish-behavior_loader)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Xml Network Widgets Multimedia MultimediaWidgets OpenGL)
target_link_libraries (${target} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Network Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets Qt5::OpenGL)

find_package(Boost REQUIRED filesystem program_options)
target_include_directories(${target} PUBLIC ${Boost_INCLUDE_DIRS}) 
target_link_libraries(${target} ${Boost_LIBRARIES})

option(WITH_PYLON5 "Support Pylon 5 cameras" $ENV{WITH_PYLON5})
if(WITH_PYLON5)
    find_package(Pylon5)
    if(Pylon5_FOUND)
        message("Pylon5 support enabled")
        target_compile_definitions(${target} PRIVATE HAS_PYLON=1)
        target_link_libraries(${target} Pylon5::Base Pylon5::Utility Pylon5::GenAPI Pylon5::GCBase)
    endif()
endif()

IF("${HMNVLibDir}" MATCHES "Not Found")
ELSE()
	target_link_libraries(${target} ${HMNVLibDir}/lib/NvEncInterace.lib)
	target_compile_definitions(${target} PRIVATE WITH_CUDA=1)
ENDIF()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(${target} stdc++fs)
endif()

message(STATUS "Configured CV version=${OpenCV_VERSION}")
message(STATUS "Configured QT version=${Qt5Core_VERSION}")
message(STATUS "Configured Boost version=${Boost_LIB_VERSION}")
add_definitions( -DMyQT_VERSION="${Qt5Core_VERSION}" )
add_definitions( -DMyCV_VERSION="${OpenCV_VERSION}" )
add_definitions( -DMyBT_VERSION="${Boost_LIB_VERSION}" )

include(Locate)
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    install(TARGETS ${target} DESTINATION .)

    option(INSTALL_REQUIRED_SYSTEM_LIBRARIES "Install required system libraries" OFF)
    if(INSTALL_REQUIRED_SYSTEM_LIBRARIES)
        set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
        include(InstallRequiredSystemLibraries)
    endif()

    if(NOT CMAKE_GENERATOR MATCHES "Visual Studio")
        include(WinDeployQt5)
        deploy_qt5(${target} DESTINATION .)
    endif()

    set(INSTALL_CUDA_COMPONENTS "" CACHE STRING "CUDA components to install")
    locate_cuda(shared_libraries COMPONENTS ${INSTALL_CUDA_COMPONENTS})

    set(INSTALL_OPENCV_COMPONENTS "" CACHE STRING "OpenCV components to install")
    locate_opencv(shared_libraries COMPONENTS ${INSTALL_OPENCV_COMPONENTS})

    set(INSTALL_SHARED_LIBRARIES "" CACHE STRING "Additional shared libraries to install")
    locate_shared_library(shared_libraries ${INSTALL_SHARED_LIBRARIES})

    locate_shared_library(shared_libraries biotracker-interfaces)

    install(FILES ${shared_libraries} DESTINATION .)
endif()

set(CPACK_PACKAGE_NAME "${target}")
set(CPACK_PACKAGE_VENDOR "Biorobotics Lab / FU Berlin")

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    option(PACKAGE_WINDOWS_MSI "Create .msi package" OFF)
    if(PACKAGE_WINDOWS_MSI)
        set(CPACK_GENERATOR "WIX")
        set(CPACK_WIX_UPGRADE_GUID "e47473db-3d71-4c42-8a58-d3eac87aa0bd")

        set(CPACK_PACKAGE_INSTALL_DIRECTORY "RoboFish/${target}")

        set(CPACK_WIX_PROGRAM_MENU_FOLDER "RoboFish")

        set_property(INSTALL "$<TARGET_FILE_NAME:${target}>"
            PROPERTY CPACK_START_MENU_SHORTCUTS "${target}"
        )

        set_property(INSTALL "$<TARGET_FILE_NAME:${target}>"
            PROPERTY CPACK_DESKTOP_SHORTCUTS "${target}"
        )
		
	    set(CPACK_WIX_LICENSE_RTF ${CMAKE_SOURCE_DIR}/Src/cmake/WIX/licence.rtf)
	    set(CPACK_WIX_TEMPLATE "${CMAKE_SOURCE_DIR}/Src/cmake/WIX/WIX.template.in")
    endif()
endif()

include(CPack)

target_sources(${target}
PRIVATE
    "BioTracker3App.cpp"
    "GuiContext.cpp"
    "main.cpp"
    "guiresources.qrc"
    "Controller/IControllerCfg.cpp"
    "Controller/ControllerAnnotations.cpp"
    "Controller/ControllerAreaDescriptor.cpp"
    "Controller/ControllerCommands.cpp"
    "Controller/ControllerCoreParameter.cpp"
    "Controller/ControllerDataExporter.cpp"
    "Controller/ControllerGraphicScene.cpp"
    "Controller/ControllerMainWindow.cpp"
    "Controller/ControllerNotifications.cpp"
    "Controller/ControllerPlayer.cpp"
    "Controller/ControllerPlugin.cpp"
    "Controller/ControllerTextureObject.cpp"
    "Controller/ControllerTrackedComponentCore.cpp"
    "Controller/null_Controller.cpp"
    "IStates/IPlayerState.cpp"
    "Model/AreaDescriptor/AreaInfo.cpp"
    "Model/AreaDescriptor/AreaInfoElement.cpp"
    "Model/AreaDescriptor/AreaMemory.cpp"
    "Model/AreaDescriptor/Rectification.cpp"
    "Model/DataExporters/DataExporterCSV.cpp"
    "Model/DataExporters/DataExporterGeneric.cpp"
    "Model/DataExporters/DataExporterJson.cpp"
    "Model/DataExporters/DataExporterSerialize.cpp"
    "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.cpp"
    "Model/MediaPlayerStateMachine/PlayerStates/PStateGoToFrame.cpp"
    "Model/MediaPlayerStateMachine/PlayerStates/PStateInitial.cpp"
    "Model/MediaPlayerStateMachine/PlayerStates/PStateInitialStream.cpp"
    "Model/MediaPlayerStateMachine/PlayerStates/PStatePause.cpp"
    "Model/MediaPlayerStateMachine/PlayerStates/PStatePlay.cpp"
    "Model/MediaPlayerStateMachine/PlayerStates/PStateStepBack.cpp"
    "Model/MediaPlayerStateMachine/PlayerStates/PStateStepForw.cpp"
    "Model/MediaPlayerStateMachine/PlayerStates/PStateWait.cpp"
    "Model/UndoCommands/TrackCommands.cpp"
    "Model/Annotations.cpp"
    "Model/BioTracker3ProxyMat.cpp"
    "Model/CoreParameter.cpp"
    "Model/ImageStream.cpp"
    "Model/MediaPlayer.cpp"
    "Model/null_Model.cpp"
    "Model/TextureObject.cpp"
    "util/CLIcommands.cpp"
    "util/VideoCoder.cpp"
    "util/Config.cpp"
    "View/AreaDesciptor/AreaDescriptor.cpp"
    "View/AreaDesciptor/EllipseDescriptor.cpp"
    "View/AreaDesciptor/RectDescriptor.cpp"
    "View/AnnotationsView.cpp"
    "View/CameraDevice.cpp"
    "View/ComponentShape.cpp"
    "View/CoreParameterView.cpp"
    "View/GLVideoView.cpp"
    "View/GraphicsScene.cpp"
    "View/GraphicsView.cpp"
    "View/MainWindow.cpp"
    "View/NotificationLogBrowser.cpp"
    "View/SettingsWindow.cpp"
    "View/TextureObjectView.cpp"
    "View/TrackedComponentView.cpp"
    "View/TrackedImageView.cpp"
    "View/VideoControllWidget.cpp"
    "View/Utility/RotationHandle.cpp"
    "View/Utility/SwitchButton.cpp"
    "View/Utility/Tracer.cpp"
)

if(WITH_PYLON5)
    target_sources(${target}
    PRIVATE
        "util/camera/pylon.cpp"
    )
endif()



