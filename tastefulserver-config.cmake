
# TASTEFULSERVER_FOUND

# TASTEFULSERVER_INCLUDES
# TASTEFULSERVER_LIBRARIES

# TASTEFULSERVER_LIBRARY
# TASTEFULSERVER_LIBRARY_RELEASE
# TASTEFULSERVER_LIBRARY_DEBUG
# TASTEFULSERVER_INCLUDE_DIR

include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(TASTEFULSERVER_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{TASTEFULSERVER_DIR}" ENVTASTEFULSERVER_DIR)

set(LIB_PATHS   
    ${TASTEFULSERVER_DIR}/build
    ${TASTEFULSERVER_DIR}/build/Release
    ${TASTEFULSERVER_DIR}/build/Debug
    ${TASTEFULSERVER_DIR}/build-release
    ${TASTEFULSERVER_DIR}/build-debug
    ${ENVTASTEFULSERVER_DIR}/lib
    ${TASTEFULSERVER_DIR}/lib
    ${ENVPROGRAMFILES}/tastefulserver/lib
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
)

macro (find LIB_NAME HEADER)
    set(HINT_PATHS ${ARGN})

    if (${LIB_NAME} STREQUAL "tastefulserver")
        set(LIB_NAME_UPPER TASTEFULSERVER)
        set(LIBNAME tastefulserver)
    else()
        string(TOUPPER TASTEFULSERVER_${LIB_NAME} LIB_NAME_UPPER)
        set(LIBNAME tastefulserver-${LIB_NAME})
    endif()

    find_path(${LIB_NAME_UPPER}_INCLUDE_DIR ${HEADER}
        ${ENVTASTEFULSERVER_DIR}/include
        ${ENVTASTEFULSERVER_DIR}/source/${LIBNAME}/include
        ${TASTEFULSERVER_DIR}/include
        ${TASTEFULSERVER_DIR}/source/${LIBNAME}/include
        ${ENVPROGRAMFILES}/tastefulserver/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where ${header} resides")

    find_library(${LIB_NAME_UPPER}_LIBRARY_RELEASE
        NAMES ${LIBNAME}
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} library")
    find_library(${LIB_NAME_UPPER}_LIBRARY_DEBUG
        NAMES ${LIBNAME}d
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} debug library")
    
    if(${LIB_NAME_UPPER}_LIBRARY_RELEASE AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()

    set(TASTEFULSERVER_INCLUDES  ${TASTEFULSERVER_INCLUDES}  ${${LIB_NAME_UPPER}_INCLUDE_DIR})
    set(TASTEFULSERVER_LIBRARIES ${TASTEFULSERVER_LIBRARIES} ${${LIB_NAME_UPPER}_LIBRARY})

    # DEBUG
    #message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    #message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    #message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    #message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

endmacro()

find(tastefulserver   tastefulserver/tastefulserver_api.h             ${LIB_PATHS})

# DEBUG
#message("TASTEFULSERVER_INCLUDES  = ${TASTEFULSERVER_INCLUDES}")
#message("TASTEFULSERVER_LIBRARIES = ${TASTEFULSERVER_LIBRARIES}")

find_package_handle_standard_args(TASTEFULSERVER DEFAULT_MSG TASTEFULSERVER_LIBRARIES TASTEFULSERVER_INCLUDES)
mark_as_advanced(TASTEFULSERVER_FOUND)
