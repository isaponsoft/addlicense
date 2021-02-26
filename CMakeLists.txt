cmake_minimum_required(VERSION 3.0.2)
project(addlicense)

set(AMTRS_APPNAME "addlicense")

if (NOT libAmtrs_DIR)
	set(libAmtrs_DIR ${CMAKE_CURRENT_BINARY_DIR}/libamtrs)
endif ()
include(${libAmtrs_DIR}/template.app-console.cmake)


install(TARGETS ${AMTRS_APPNAME} DESTINATION bin)
