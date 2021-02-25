cmake_minimum_required(VERSION 3.0.2)
project(addlicense)

set(AMTRS_APPNAME "addlicense")

set(libAmtrs_DIR ${CMAKE_CURRENT_BINARY_DIR}/libamtrs)
include(${libAmtrs_DIR}/template.app-console.cmake)


install(TARGETS ${AMTRS_APPNAME} DESTINATION bin)
