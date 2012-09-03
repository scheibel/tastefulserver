project(HttpServer)

find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
include(${QT_USE_FILE})

include_directories("../include/")

add_definitions(-std=c++0x)

add_executable(HttpServer "http-server/main.cpp")

target_link_libraries(HttpServer ${QT_LIBRARIES} TastefulServer)

###########################################

project(HttpsServer)

find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
include(${QT_USE_FILE})

include_directories("../include/")

add_definitions(-std=c++0x)

add_executable(HttpsServer "https-server with cookies/main.cpp")

target_link_libraries(HttpsServer ${QT_LIBRARIES} TastefulServer)