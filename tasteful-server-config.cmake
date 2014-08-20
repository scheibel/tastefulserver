
# TASTEFUL_SERVER_FOUND

# TASTEFUL_SERVER_INCLUDES
# TASTEFUL_SERVER_LIBRARIES

# TASTEFUL_SERVER_LIBRARY
# TASTEFUL_SERVER_LIBRARY_RELEASE
# TASTEFUL_SERVER_LIBRARY_DEBUG
# TASTEFUL_SERVER_INCLUDE_DIR

include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(TASTEFUL_SERVER_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{TASTEFUL_SERVER_DIR}" ENVTASTEFUL_SERVER_DIR)

set(LIB_PATHS   
    ${TASTEFUL_SERVER_DIR}/build
    ${TASTEFUL_SERVER_DIR}/build/Release
    ${TASTEFUL_SERVER_DIR}/build/Debug
    ${TASTEFUL_SERVER_DIR}/build-release
    ${TASTEFUL_SERVER_DIR}/build-debug
    ${ENVTASTEFUL_SERVER_DIR}/lib
    ${TASTEFUL_SERVER_DIR}/lib
    ${ENVPROGRAMFILES}/tasteful-server/lib
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

    if (${LIB_NAME} STREQUAL "tasteful-server")
        set(LIB_NAME_UPPER TASTEFUL_SERVER)
        set(LIBNAME tasteful-server)
    else()
        string(TOUPPER TASTEFUL_SERVER_${LIB_NAME} LIB_NAME_UPPER)
        set(LIBNAME tasteful-server-${LIB_NAME})
    endif()

    find_path(${LIB_NAME_UPPER}_INCLUDE_DIR ${HEADER}
        ${ENVTASTEFUL_SERVER_DIR}/include
        ${ENVTASTEFUL_SERVER_DIR}/source/${LIBNAME}/include
        ${TASTEFUL_SERVER_DIR}/include
        ${TASTEFUL_SERVER_DIR}/source/${LIBNAME}/include
        ${ENVPROGRAMFILES}/tasteful-server/include
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

    set(TASTEFUL_SERVER_INCLUDES  ${TASTEFUL_SERVER_INCLUDES}  ${${LIB_NAME_UPPER}_INCLUDE_DIR})
    set(TASTEFUL_SERVER_LIBRARIES ${TASTEFUL_SERVER_LIBRARIES} ${${LIB_NAME_UPPER}_LIBRARY})

    # DEBUG
    #message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    #message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    #message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    #message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

endmacro()

find(tasteful-server   tasteful-server/tasteful-server_api.h             ${LIB_PATHS})

# DEBUG
#message("TASTEFUL_SERVER_INCLUDES  = ${TASTEFUL_SERVER_INCLUDES}")
#message("TASTEFUL_SERVER_LIBRARIES = ${TASTEFUL_SERVER_LIBRARIES}")

find_package_handle_standard_args(TASTEFUL_SERVER DEFAULT_MSG TASTEFUL_SERVER_LIBRARIES TASTEFUL_SERVER_INCLUDES)
mark_as_advanced(TASTEFUL_SERVER_FOUND)
