project(HttpsServer)

find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
include(${QT_USE_FILE})

include_directories("../../include/")

add_definitions(-std=c++0x -Wreturn-type -Werror=return-type)

add_executable(HttpsServer "main.cpp")

target_link_libraries(HttpsServer ${QT_LIBRARIES} TastefulServer)
