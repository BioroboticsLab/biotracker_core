
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(Pylon5_LIBRARY_DIR "/opt/pylon5/lib64")
    else()
        set(Pylon5_LIBRARY_DIR "/opt/pylon5/lib32")
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(Pylon5_LIBRARY_DIR "$ENV{PYLON5_DIR}/lib/x64")
    else()
        set(Pylon5_LIBRARY_DIR "$ENV{PYLON5_DIR}/lib/Win32")
    endif()
endif()

find_path(Pylon5_INCLUDE_DIR pylon/PylonBase.h
          PATHS
          /opt/pylon5/include
          "$ENV{PYLON5_DIR}/include")

find_library(Pylon5_Base_LIBRARY
             NAMES
                pylonbase
                PylonBase_v5_2
             PATHS ${Pylon5_LIBRARY_DIR})

find_library(Pylon5_Utility_LIBRARY
             NAMES
                pylonutility
                PylonUtility_v5_2
             PATHS ${Pylon5_LIBRARY_DIR})

find_library(Pylon5_GenAPI_LIBRARY
             NAMES
                GenApi_gcc_v3_1_Basler_pylon
                GenApi_MD_VC141_v3_1_Basler_pylon
             PATHS ${Pylon5_LIBRARY_DIR})

find_library(Pylon5_GCBase_LIBRARY
             NAMES
                GCBase_gcc_v3_1_Basler_pylon
                GCBase_MD_VC141_v3_1_Basler_pylon
             PATHS ${Pylon5_LIBRARY_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Pylon5 DEFAULT_MSG
                                  Pylon5_INCLUDE_DIR Pylon5_Base_LIBRARY Pylon5_Utility_LIBRARY Pylon5_GenAPI_LIBRARY Pylon5_GCBase_LIBRARY)

mark_as_advanced(Pylon5_INCLUDE_DIR Pylon5_Base_LIBRARY Pylon5_Utility_LIBRARY Pylon5_GenAPI_LIBRARY Pylon5_GCBase_LIBRARY)

set(Pylon5_LIBRARIES ${Pylon5_Base_LIBRARY} ${Pylon5_Utility_LIBRARY} ${Pylon5_GenAPI_LIBRARY} ${Pylon5_GCBase_LIBRARY})
set(Pylon5_INCLUDE_DIRS ${Pylon5_INCLUDE_DIR})

add_library(Pylon5::Base UNKNOWN IMPORTED)
set_target_properties (Pylon5::Base PROPERTIES IMPORTED_LOCATION ${Pylon5_Base_LIBRARY})
set_target_properties (Pylon5::Base PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${Pylon5_INCLUDE_DIRS})

add_library(Pylon5::Utility UNKNOWN IMPORTED)
set_target_properties (Pylon5::Utility PROPERTIES IMPORTED_LOCATION ${Pylon5_Utility_LIBRARY})
set_target_properties (Pylon5::Utility PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${Pylon5_INCLUDE_DIRS})

add_library(Pylon5::GenAPI UNKNOWN IMPORTED)
set_target_properties (Pylon5::GenAPI PROPERTIES IMPORTED_LOCATION ${Pylon5_GenAPI_LIBRARY})
set_target_properties (Pylon5::GenAPI PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${Pylon5_INCLUDE_DIRS})

add_library(Pylon5::GCBase UNKNOWN IMPORTED)
set_target_properties (Pylon5::GCBase PROPERTIES IMPORTED_LOCATION ${Pylon5_GCBase_LIBRARY})
set_target_properties (Pylon5::GCBase PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${Pylon5_INCLUDE_DIRS})
