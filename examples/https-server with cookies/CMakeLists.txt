project(HttpsServer)

find_package(Qt5Core REQUIRED)

include_directories("../../include/")

add_definitions(-std=c++0x -Wreturn-type -Werror=return-type)

add_executable(HttpsServer "main.cpp")

target_link_libraries(HttpsServer TastefulServer)

qt5_use_modules(HttpsServer Network Xml)
