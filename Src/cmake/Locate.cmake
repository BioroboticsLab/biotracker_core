
function(locate_shared_library_anyof VAR)
    foreach(name ${ARGN})
        set(_filename "${CMAKE_SHARED_LIBRARY_PREFIX}${name}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        message(STATUS "Locating ${_filename}")
        if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
            execute_process(
                COMMAND where.exe "${_filename}"
                OUTPUT_VARIABLE _matches
                OUTPUT_STRIP_TRAILING_WHITESPACE
                RESULT_VARIABLE _res
            )
            if("${_res}" STREQUAL "0")
                string(REGEX REPLACE "\n" ";" _matches "${_matches}")
                list(GET _matches 0 _match)
                string(REGEX REPLACE "\\\\" "/" _match "${_match}")

                if(DEFINED ${VAR})
                    list(APPEND ${VAR} "${_match}")
                    set(${VAR} ${${VAR}} PARENT_SCOPE)
                else()
                    set(${VAR} "${_match}" PARENT_SCOPE)
                endif()
                return()
            endif()
        else()
            message(FATAL_ERROR "Platform not supported")
        endif()
    endforeach()
    message(FATAL_ERROR "Could not locate any of ${ARGN}")
endfunction()

function(locate_shared_library VAR)
    foreach(name ${ARGN})
        set(_filename "${CMAKE_SHARED_LIBRARY_PREFIX}${name}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        message(STATUS "Locating ${_filename}")
        if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
            execute_process(
                COMMAND where.exe "${_filename}"
                OUTPUT_VARIABLE _matches
                OUTPUT_STRIP_TRAILING_WHITESPACE
                RESULT_VARIABLE _res
            )
            if(NOT "${_res}" STREQUAL "0")
                message(FATAL_ERROR "Could not locate ${_filename}")
            endif()
            string(REGEX REPLACE "\n" ";" _matches "${_matches}")
            list(GET _matches 0 _match)
            string(REGEX REPLACE "\\\\" "/" _match "${_match}")

            if(DEFINED ${VAR})
                list(APPEND ${VAR} "${_match}")
                set(${VAR} ${${VAR}} PARENT_SCOPE)
            else()
                set(${VAR} "${_match}" PARENT_SCOPE)
            endif()
        else()
            message(FATAL_ERROR "Platform not supported")
        endif()
    endforeach()
endfunction()

function(locate_opencv VAR)
    cmake_parse_arguments(ARG "" "" "COMPONENTS" ${ARGN})

    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        set(version "${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")

        if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
            set(platform "64")
        else()
            set(platform "32")
        endif()

        foreach(component ${ARG_COMPONENTS})
            locate_shared_library_anyof(${VAR} "opencv_${component}" "opencv_${component}${version}" "opencv_${component}${version}_${platform}")
        endforeach()
        set(${VAR} ${${VAR}} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Platform not supported")
    endif()
endfunction()

function(locate_cuda VAR)
    cmake_parse_arguments(ARG "" "" "COMPONENTS" ${ARGN})

    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
            set(suffix "64_${CUDA_VERSION_MAJOR}")
        else()
            set(suffix "32_${CUDA_VERSION_MAJOR}")
        endif()

        foreach(component ${ARG_COMPONENTS})
            locate_shared_library_anyof(${VAR} "${component}${suffix}" "${component}${suffix}${CUDA_VERSION_MINOR}")
        endforeach()
        set(${VAR} ${${VAR}} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Platform not supported")
    endif()
endfunction()

function(locate_cudnn VAR)
    cmake_parse_arguments(ARG "" "" "COMPONENTS" ${ARGN})

    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
            set(suffix "64_${version}")
        else()
            set(suffix "32_${version}")
        endif()

        locate_shared_library(${VAR} "cudnn${suffix}")
        set(${VAR} ${${VAR}} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Platform not supported")
    endif()
endfunction()
