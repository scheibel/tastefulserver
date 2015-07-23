set(target tastefulserver-test)
message(STATUS "Test ${target}")

# Includes

find_package(Qt5Core    5.0 REQUIRED)
find_package(Qt5Network 5.0 REQUIRED)

include_directories(   
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/tastefulserver/include
)

# Libraries

set(libs
    ${GMOCK_LIBRARIES}
    ${GTEST_LIBRARIES}
    tastefulserver
    Qt5::Core
    Qt5::Network
)

# Sources

set(sources
    main.cpp
    dummy_test.cpp
)

# Build executable

add_executable(${target} ${sources})

target_link_libraries(${target} ${libs})

target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")
