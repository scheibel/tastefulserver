cmake_minimum_required(VERSION 2.6)

project(TastefulServer)

add_subdirectory(examples)

find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
include(${QT_USE_FILE})

include_directories("include/")

add_definitions(-std=c++0x)

QT4_WRAP_CPP(Server_moc_sources
	include/internal/ThreadPool
	include/TcpServer
	include/TcpDataHandler
	include/internal/TcpConnectionHandler
)

set(Server_sources
	core/ThreadPool.cpp
	core/ByteArrayStream.cpp
	core/QVariantTree.cpp
	
	server/TcpServer.cpp
	server/TcpDataHandler.cpp
	server/TcpConnectionHandler.cpp
	
	http/httpUtil.cpp
	http/HttpMethod.cpp
	http/ContentType.cpp
	http/HttpHeader.cpp
	http/HttpEntity.cpp
	http/HttpVersion.cpp
	http/HttpMessage.cpp
	http/RequestParameters.cpp
	http/UploadedFile.cpp
	http/HttpRequest.cpp
	http/HttpResponse.cpp
	http/Cookie.cpp
	http/MultiPart.cpp
	http/HttpDataHandler.cpp
	http/HttpServer.cpp
	http/HttpsServer.cpp
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
