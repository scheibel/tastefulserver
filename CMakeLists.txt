cmake_minimum_required(VERSION 2.6)

project(TastefulServer)

option(WITH_EXAMPLES "Compile with examples" OFF)

if (WITH_EXAMPLES)
	message (STATUS "Compile Tasteful Server WITH examples")
	
	add_subdirectory(examples)
else (WITH_EXAMPLES)
	message (STATUS "Compile Tasteful Server WITHOUT examples")
endif (WITH_EXAMPLES)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Core REQUIRED)

include_directories("include/" "src/")

add_definitions(-std=c++0x -Wreturn-type -Werror=return-type)

set(Server_sources
	src/core/ByteArrayStream.cpp
	src/core/QVariantTree.h
	src/core/QVariantTree.cpp
	src/core/Task.cpp
	src/core/ThreadPool.h
	src/core/ThreadPool.cpp
	
	src/server/SocketCreation.h
	src/server/SocketCreation.cpp
	src/server/ConnectionHandler.cpp
	src/server/TcpServer.cpp
	
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
	include/ByteArrayStream.h
	include/ByteArrayStream
	include/ContentType.h
	include/ContentType
	include/Cookie.h
	include/Cookie
	include/ConnectionHandler.h
	include/ConnectionHandler
	include/http.h
	include/http
	include/HttpEntity.h
	include/HttpEntity
	include/HttpHandler.h
	include/HttpHandler
	include/HttpHeader.h
	include/HttpHeader
	include/HttpMessage.h
	include/HttpMessage
	include/HttpMethod.h
	include/HttpMethod
	include/HttpRequest.h
	include/HttpRequest
	include/HttpResponse.h
	include/HttpResponse
	include/HttpServer.h
	include/HttpServer
	include/HttpsServer.h
	include/HttpsServer
	include/httpStatusCodes.h
	include/httpStatusCodes
	include/httpUtil.h
	include/httpUtil
	include/HttpVersion.h
	include/HttpVersion
	include/MultiPart.h
	include/MultiPart
	include/RequestParameters.h
	include/RequestParameters
	include/Task.h
	include/Task
	include/TcpServer.h
	include/TcpServer
	include/UploadedFile.h
	include/UploadedFile
)

add_library(TastefulServer SHARED ${Server_sources} ${Server_headers})

qt5_use_modules(TastefulServer Network Xml)

#target_link_libraries(TastefulServer ${QT_LIBRARIES})

install(FILES ${Server_headers} DESTINATION include/tastefulserver/)
install(FILES ${Server_internal_headers} DESTINATION include/tastefulserver/internal)
install(TARGETS TastefulServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(FILES "${CMAKE_SOURCE_DIR}/FindTastefulServer.cmake" DESTINATION ${CMAKE_ROOT}/Modules)
