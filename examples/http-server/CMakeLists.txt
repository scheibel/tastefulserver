project(HttpServer)

find_package(Qt5Core REQUIRED)

include_directories("../../include/")

add_definitions(-std=c++0x -Wreturn-type -Werror=return-type)

add_executable(HttpServer "main.cpp")

target_link_libraries(HttpServer TastefulServer)

qt5_use_modules(HttpServer Network Xml)
