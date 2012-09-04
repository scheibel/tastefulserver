cmake_minimum_required(VERSION 2.6)

project(TastefulServer)

option(WITH_EXAMPLES "Compile with examples" OFF)

if (WITH_EXAMPLES)
	message ("-- Compile Tasteful Server WITH examples")
	
	add_subdirectory(examples)
else (WITH_EXAMPLES)
	message ("-- Compile Tasteful Server WITHOUT examples")
endif (WITH_EXAMPLES)

find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
include(${QT_USE_FILE})

include_directories("include/")

add_definitions(-std=c++0x)

QT4_WRAP_CPP(Server_moc_sources
	include/internal/ThreadPool
	include/TcpServer
	include/internal/ConnectionHandler
)

set(Server_sources
	src/core/ThreadPool.cpp
	src/core/ByteArrayStream.cpp
	src/core/QVariantTree.cpp
	
	src/server/SocketCreation.cpp
	src/server/TcpServer.cpp
	src/server/ConnectionHandler.cpp
	
	src/http/httpUtil.cpp
	src/http/HttpMethod.cpp
	src/http/ContentType.cpp
	src/http/HttpHeader.cpp
	src/http/HttpEntity.cpp
	src/http/HttpVersion.cpp
	src/http/HttpMessage.cpp
	src/http/RequestParameters.cpp
	src/http/UploadedFile.cpp
	src/http/HttpRequest.cpp
	src/http/HttpResponse.cpp
	src/http/Cookie.cpp
	src/http/MultiPart.cpp
	src/http/HttpHandler.cpp
	src/http/HttpServer.cpp
	src/http/HttpsServer.cpp
)

set(Server_headers
	include/ContentType
	include/Cookie
	include/http
	include/HttpDataHandler
	include/HttpEntity
	include/HttpHeader
	include/HttpMessage
	include/HttpMethod
	include/HttpRequest
	include/HttpResponse
	include/HttpServer
	include/HttpsServer
	include/httpStatusCodes
	include/httpUtil
	include/HttpVersion
	include/MultiPart
	include/RequestParameters
	include/TcpDataHandler
	include/TcpServer
	include/UploadedFile
)

set(Server_internal_headers
	include/internal/ByteArrayStream
	include/internal/ThreadPool
	include/internal/QVariantTree
	include/internal/TcpConnectionHandler
)

add_library(TastefulServer SHARED ${Server_sources} ${Server_moc_sources})

target_link_libraries(TastefulServer ${QT_LIBRARIES})

install(FILES ${Server_headers} DESTINATION include/tastefulserver/)
install(FILES ${Server_internal_headers} DESTINATION include/tastefulserver/internal)
install(TARGETS TastefulServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
install(FILES "${CMAKE_SOURCE_DIR}/FindTastefulServer.cmake" DESTINATION ${CMAKE_ROOT}/Modules)
