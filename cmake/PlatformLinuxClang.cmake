message(STATUS "Configuring for platform Linux/GCC.")


# Enable C++11 support

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")

include_directories("/usr/include/c++/v1/")

set(LINUX_COMPILE_DEFS
	LINUX	                  # Linux system
        PIC		          # Position-independent code
	_REENTRANT                # Reentrant code
)
set(DEFAULT_COMPILE_DEFS_DEBUG
    ${LINUX_COMPILE_DEFS}
    _DEBUG	                  # Debug build
)
set(DEFAULT_COMPILE_DEFS_RELEASE
    ${LINUX_COMPILE_DEFS}
    NDEBUG	                  # Release build
)

set(LINUX_COMPILE_FLAGS "-fvisibility=hidden -pthread -pipe -fPIC -Werror -Wall -pedantic -Wextra -Wfloat-equal -Wcast-qual -Wcast-align -Wno-error=float-equal -Wno-error=switch -Wreturn-type -fno-exceptions -Wno-mismatched-tags -Wno-unsequenced -Wno-sign-conversion -Wno-unused-function -Wno-missing-braces")
# pthread       -> use pthread library
# no-rtti       -> disable c++ rtti
# no-exceptions -> disable exception handling
# pipe          -> use pipes
# fPIC          -> use position independent code
# -Wreturn-type -Werror=return-type -> missing returns in functions and methods are handled as errors which stops the compilation
# -Wshadow -> e.g. when a parameter is named like a member, too many warnings, disabled for now
# -fvisibility=hidden -> only export symbols with __attribute__
set(LINUX_LINKER_FLAGS "-pthread")

set(DEFAULT_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})

set(DEFAULT_LINKER_FLAGS_RELEASE ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS_DEBUG ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS ${LINUX_LINKER_FLAGS})

# Add platform specific libraries for linking
set(EXTRA_LIBS "")
