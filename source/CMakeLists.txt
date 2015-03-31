# Generate meta-header
configure_file(tasteful-server-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/tasteful-server-version.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)


#
# Qt5
#

# good resource: http://www.kdab.com/using-cmake-with-qt-5/

# http://qt-project.org/forums/viewthread/30006/
if(MSVC)
    cmake_policy(SET CMP0020 NEW)
endif()

set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)

# Probably works in the next cmake release -> http://www.cmake.org/Bug/view.php?id=13788
# What we do not want is automocs beside the project -> http://www.cmake.org/Bug/view.php?id=13688
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)


#
# Projects
#

# Libraries
set(IDE_FOLDER "")
add_subdirectory(tastefulserver)

# Examples
set(IDE_FOLDER "Examples")
add_subdirectory(examples)

# Tests
# set(IDE_FOLDER "Tests")
# add_subdirectory(tests)
