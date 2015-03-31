
# Target
set(target tastefulserver)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(Qt5Core 5.0 REQUIRED)

#
# Includes
#

include_directories(
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DTASTEFUL_SERVER_STATIC")
else()
    add_definitions("-DTASTEFUL_SERVER_EXPORTS")
endif()

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
	${source_path}/core/Task.cpp
	${source_path}/core/ByteStream.cpp
	${source_path}/core/ThreadPool.cpp
	${source_path}/core/ThreadPool.h
	${source_path}/http/HttpSocketHandler.cpp
	${source_path}/http/HttpMessage.cpp
	${source_path}/http/HttpVersion.cpp
	${source_path}/http/httpUtil.cpp
	${source_path}/http/UploadedFile.cpp
	${source_path}/http/HttpsSocket.cpp
	${source_path}/http/HttpEntity.cpp
	${source_path}/http/ContentType.cpp
	${source_path}/http/Cookie.cpp
	${source_path}/http/RequestParameters.cpp
	${source_path}/http/HttpResponse.cpp
	${source_path}/http/HttpRequest.cpp
	${source_path}/http/MultiPart.cpp
	${source_path}/http/HttpSocket.cpp
	${source_path}/http/HttpMethod.cpp
	${source_path}/http/HttpHeader.cpp
	${source_path}/http/HttpRequestParser.cpp
	${source_path}/server/WebSocketServer.cpp
	${source_path}/server/Connection.cpp
	${source_path}/server/HttpServer.cpp
	${source_path}/server/HttpCallbackServer.cpp
	${source_path}/server/HttpWebSocketServer.cpp
	${source_path}/server/TcpServer.cpp
	${source_path}/server/HttpsCallbackServer.cpp
	${source_path}/server/AbstractSocket.cpp
	${source_path}/server/HttpsServer.cpp
	${source_path}/websocket/WebSocketFrame.cpp
	${source_path}/websocket/WebSocketHandler.cpp
	${source_path}/websocket/WebSocket.cpp
	${source_path}/websocket/WebSocketFrameParser.cpp
)

set(api_includes
	${include_path}/AbstractSocket.h
	${include_path}/ByteStream.h
	${include_path}/Connection.h
	${include_path}/ContentType.h
	${include_path}/Cookie.h
	${include_path}/HttpCallbackServer.h
	${include_path}/HttpEntity.h
	${include_path}/http.h
	${include_path}/HttpHeader.h
	${include_path}/HttpMessage.h
	${include_path}/HttpMethod.h
	${include_path}/HttpRequest.h
	${include_path}/HttpRequestParser.h
	${include_path}/HttpResponse.h
	${include_path}/HttpsCallbackServer.h
	${include_path}/HttpServer.h
	${include_path}/HttpSocket.h
	${include_path}/HttpSocketHandler.h
	${include_path}/HttpsServer.h
	${include_path}/HttpsSocket.h
	${include_path}/httpStatusCodes.h
	${include_path}/httpUtil.h
	${include_path}/HttpVersion.h
	${include_path}/HttpWebSocketServer.h
	${include_path}/MultiPart.h
	${include_path}/RequestParameters.h
	${include_path}/Task.h
        ${include_path}/tasteful-server_api.h
	${include_path}/TcpServer.h
	${include_path}/UploadedFile.h
	${include_path}/WebSocketFrame.h
	${include_path}/WebSocketFrameParser.h
	${include_path}/WebSocket.h
	${include_path}/WebSocketHandler.h
	${include_path}/WebSocketServer.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()

target_link_libraries(${target} ${libs})
qt5_use_modules(${target} Core Network)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            ${include_path})

#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
