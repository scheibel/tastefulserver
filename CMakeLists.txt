cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

set(META_PROJECT_NAME        "TastefulServer")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "1")
set(META_VERSION_PATCH       "0")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_AUTHOR_ORGANIZATION "Tasteful Revolution")
set(META_AUTHOR_DOMAIN       "https://github.com/scheibel/tastefulserver/")

option(OPTION_BUILD_STATIC   "Build static libraries" OFF)
option(OPTION_BUILD_WITH_EXAMPLES "Compile with examples" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(TastefulServer_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (OPTION_BUILD_WITH_EXAMPLES)
	message (STATUS "Compile Tasteful Server WITH examples")
else (OPTION_BUILD_WITH_EXAMPLES)
	message (STATUS "Compile Tasteful Server WITHOUT examples.")
	message (STATUS "To compile Tasteful Server with examples, add -DOPTION_BUILD_WITH_EXAMPLES=On to the cmake command.")
endif (OPTION_BUILD_WITH_EXAMPLES)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(OPTION_LIMIT_CONFIGS)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)
endif()

project(${META_PROJECT_NAME} C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_AUTOMOC ON)
include(cmake/Custom.cmake)

# PLATFORM AND ARCHITECTURE

# Architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64 ON)
endif()

# Check for linux
if(UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

# Setup platform specifics (compile flags, etc., ...)
if(MSVC)
    message(STATUS "Configuring for platform Windows/MSVC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Windows/GCC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(LINUX AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Linux/GCC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
elseif(APPLE)
    message(STATUS "Configuring for platform MacOS.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
else()
    # Unsupported system/compiler
    message(WARNING "Unsupported platform/compiler combination")
endif()


# INSTALL

if(X64)
    # http://public.kitware.com/Bug/view.php?id=9094
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
endif()

set(project ${META_PROJECT_NAME})
if(WIN32)
    set(INSTALL_BIN           ".")                      # C:\Programme\<project>
    set(INSTALL_SHARED        ".")                      # C:\Programme\<project>
    set(INSTALL_LIB           "lib")                    # C:\Programme\<project>\lib
    set(INSTALL_INCLUDE       "include")                # C:\Programme\<project>\include
    set(INSTALL_DOC           "doc")                    # C:\Programme\<project>\doc
    set(INSTALL_APPLICATIONS  "")                       # Not available under Windows
    set(INSTALL_ICONS         "")                       # Not available under Windows
    set(INSTALL_INIT          "")                       # Not available under Windows
    set(INSTALL_DATA          ".")                      # C:\Programme\<project>
    set(INSTALL_EXAMPLES      ".")                      # C:\Programme\<project>
else()
    set(INSTALL_BIN           "bin")                    # /usr/[local]/bin
    set(INSTALL_SHARED        "lib")                    # /usr/[local]/lib
    set(INSTALL_LIB           "lib")                    # /usr/[local]/lib
    set(INSTALL_INCLUDE       "include")                # /usr/[local]/include
    set(INSTALL_DOC           "share/doc/${project}")   # /usr/[local]/share/doc/<project>
    set(INSTALL_APPLICATIONS  "share/applications")     # /usr/[local]/share/applications
    set(INSTALL_ICONS         "share/pixmaps")          # /usr/[local]/share/pixmaps
    set(INSTALL_INIT          "/etc/init")              # /etc/init (upstart init scripts)
    set(INSTALL_DATA          "share/${project}")       # /usr/[local]/share/<project>
    set(INSTALL_EXAMPLES      "share/${project}")       # /usr/[local]/share/<project>

    # Adjust target paths for Local installs
    if(OPTION_LOCAL_INSTALL)
        # Put binaries in root directory and keep data directory name
        set(INSTALL_BIN                       ".")           # /<INSTALL_PREFIX>
        set(INSTALL_DATA                      ".")           # /<INSTALL_PREFIX>

        # We have to change the RPATH of binaries to achieve a usable local install.
        # [TODO] For binaries, "$ORIGIN/lib" is right, so that libraries are found in ./lib.
        #        However, I have not yet tested what happens when libraries use other libraries.
        #        In that case, they might need the rpath $ORIGIN instead ...
        set(CMAKE_SKIP_BUILD_RPATH            FALSE)         # Use automatic rpath for build
        set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE)         # Use specific rpath for INSTALL
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)         # NO automatic rpath for INSTALL
        set(CMAKE_INSTALL_RPATH               "$ORIGIN/lib") # Libraries are relative to binary
    endif()
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE})

install(FILES tastefulserver-config.cmake DESTINATION ".")

# GET REPOSITORY VERSION

# Add a revision file containing the git-head tag for cpack and install
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

# Generate a shorter, googlelike variation for rev
string(SUBSTRING ${GIT_SHA1} 0 12 GIT_REV)
file(WRITE ${CMAKE_BINARY_DIR}/revision ${GIT_REV})

# [TODO] Really?? Do we want this deployed?
#install(FILES ${CMAKE_BINARY_DIR}/revision DESTINATION .)

#

find_package(Qt5Core REQUIRED)

set(target TastefulServer)
message(STATUS "lib ${target}")

set(header_path "${TastefulServer_SOURCE_DIR}/include")
set(source_path "${TastefulServer_SOURCE_DIR}/src")

include_directories(
	"include/"
	"src/"
)

set(${target}_sources
	${source_path}/core/ByteArrayStream.cpp
	${source_path}/core/QVariantTree.h
	${source_path}/core/QVariantTree.cpp
	${source_path}/core/Task.cpp
	${source_path}/core/ThreadPool.h
	${source_path}/core/ThreadPool.cpp
	
	${source_path}/server/SocketCreation.h
	${source_path}/server/SocketCreation.cpp
	${source_path}/server/ConnectionHandler.cpp
	${source_path}/server/TcpServer.cpp
	
	${source_path}/http/httpUtil.cpp
	${source_path}/http/HttpMethod.cpp
	${source_path}/http/ContentType.cpp
	${source_path}/http/HttpHeader.cpp
	${source_path}/http/HttpEntity.cpp
	${source_path}/http/HttpVersion.cpp
	${source_path}/http/HttpMessage.cpp
	${source_path}/http/RequestParameters.cpp
	${source_path}/http/UploadedFile.cpp
	${source_path}/http/HttpRequest.cpp
	${source_path}/http/HttpResponse.cpp
	${source_path}/http/Cookie.cpp
	${source_path}/http/MultiPart.cpp
	${source_path}/http/HttpHandler.cpp
	${source_path}/http/HttpServer.cpp
	${source_path}/http/HttpsServer.cpp
)

set(${target}_headers
	${header_path}/ByteArrayStream.h
	${header_path}/ByteArrayStream
	${header_path}/ContentType.h
	${header_path}/ContentType
	${header_path}/Cookie.h
	${header_path}/Cookie
	${header_path}/ConnectionHandler.h
	${header_path}/ConnectionHandler
	${header_path}/http.h
	${header_path}/http
	${header_path}/HttpEntity.h
	${header_path}/HttpEntity
	${header_path}/HttpHandler.h
	${header_path}/HttpHandler
	${header_path}/HttpHeader.h
	${header_path}/HttpHeader
	${header_path}/HttpMessage.h
	${header_path}/HttpMessage
	${header_path}/HttpMethod.h
	${header_path}/HttpMethod
	${header_path}/HttpRequest.h
	${header_path}/HttpRequest
	${header_path}/HttpResponse.h
	${header_path}/HttpResponse
	${header_path}/HttpServer.h
	${header_path}/HttpServer
	${header_path}/HttpsServer.h
	${header_path}/HttpsServer
	${header_path}/httpStatusCodes.h
	${header_path}/httpStatusCodes
	${header_path}/httpUtil.h
	${header_path}/httpUtil
	${header_path}/HttpVersion.h
	${header_path}/HttpVersion
	${header_path}/MultiPart.h
	${header_path}/MultiPart
	${header_path}/RequestParameters.h
	${header_path}/RequestParameters
	${header_path}/Task.h
	${header_path}/Task
	${header_path}/TcpServer.h
	${header_path}/TcpServer
	${header_path}/UploadedFile.h
	${header_path}/UploadedFile
)

set(tastefulserver_header_group "Header Files (API)")
set(tastefulserver_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${tastefulserver_header_group} ${${target}_headers})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${tastefulserver_source_group} ${${target}_sources})

if(OPTION_BUILD_STATIC)
    add_library(${target} ${${target}_headers} ${${target}_sources})
else()
    add_library(${target} SHARED ${${target}_headers} ${${target}_sources})
endif()

qt5_use_modules(${target} Network Xml)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            "${TastefulServer_SOURCE_DIR}/include")

# Deployment
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# EXAMPLES

if (OPTION_BUILD_WITH_EXAMPLES)
	add_subdirectory(examples)
endif (OPTION_BUILD_WITH_EXAMPLES)

# PACKAGING

add_subdirectory(packages)


# DOXYGEN

add_subdirectory(docs)