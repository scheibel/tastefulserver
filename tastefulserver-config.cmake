
# TastefulServer_FOUND
# TastefulServer_INCLUDE_DIR

# TastefulServer_LIBRARY
# TastefulServer_LIBRARY_DEBUG
# TastefulServer_LIBRARIES

if(CMAKE_CURRENT_LIST_FILE)
	get_filename_component(TastefulServer_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{TastefulServer_DIR}" ENVTS_DIR)

find_path(GLOW_INCLUDE_DIR glow/glow.h
	${ENVTS_DIR}/include
	${TastefulServer_DIR}/include
	${ENVPROGRAMFILES}/tastefulserver/include
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory where TcpServer resides")

set(LIB_PATHS 	
	${TastefulServer_DIR}/build
	${TastefulServer_DIR}/build/Release
	${TastefulServer_DIR}/build/Debug
	${TastefulServer_DIR}/build-release
	${TastefulServer_DIR}/build-debug
	${ENVTS_DIR}/lib
	${TastefulServer_DIR}/lib
	${ENVPROGRAMFILES}/tastefulserver/lib
	/usr/lib
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	/usr/lib64
	/usr/local/lib64
	/sw/lib64
	/opt/local/lib64
)

macro (find LIB_NAME)
	set(HINT_PATHS ${ARGN})

	string(TOUPPER ${LIB_NAME} LIB_NAME_UPPER)
	set(LIBNAME ${LIB_NAME})

	find_library(${LIB_NAME_UPPER}_LIBRARY
		NAMES ${LIBNAME}
		PATHS ${HINT_PATHS}
		DOC "The ${LIB_NAME} library")
	find_library(${LIB_NAME_UPPER}_LIBRARY_DEBUG
		NAMES ${LIBNAME}d
		PATHS ${HINT_PATHS}
		DOC "The ${LIB_NAME} debug library")
	
	if(${LIB_NAME_UPPER}_LIBRARY AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
		set(${LIB_NAME_UPPER}_LIBRARIES "optimized" ${${LIB_NAME_UPPER}_LIBRARY} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
	elseif(${LIB_NAME_UPPER}_LIBRARY)
		set(${LIB_NAME_UPPER}_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY})
	elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
		set(${LIB_NAME_UPPER}_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
	endif()
endmacro()

find(tastefulserver ${LIB_PATHS})

find_package_handle_standard_args(TastefulServer DEFAULT_MSG TastefulServer_LIBRARIES TastefulServer_INCLUDE_DIR)
mark_as_advanced(TastefulServer_FOUND)
