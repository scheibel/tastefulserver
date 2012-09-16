project(HttpServer)

find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
include(${QT_USE_FILE})

include_directories("../../include/")

add_definitions(-std=c++0x)

add_executable(HttpServer "main.cpp")

target_link_libraries(HttpServer ${QT_LIBRARIES} TastefulServer)
