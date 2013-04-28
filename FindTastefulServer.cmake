#
# Finds TastefulServer includes and library
#
# ${TastefulServer_FOUND}
# ${TastefulServer_INCLUDE_DIR}
# ${TastefulServer_LIBRARY}

include(FindPackageHandleStandardArgs)

find_package(Qt5Core REQUIRED)

if(WIN_32)
	message(WARNING "Windows not yet supported")
else(WIN_32)
	find_path(TastefulServer_INCLUDE_DIR TcpServer
		PATHS
		/usr/local/include/tastefulserver
		/usr/include/tastefulserver
		/opt/local/include/tastefulserver
	)
	find_library(TastefulServer_LIBRARY libTastefulServer.so
		PATHS
		/usr/local/lib
		/usr/lib
		/opt/local/lib
	)
endif(WIN_32)

find_package_handle_standard_args(TastefulServer DEFAULT_MSG TastefulServer_INCLUDE_DIR TastefulServer_LIBRARY)