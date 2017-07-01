cmake_minimum_required (VERSION 2.6)

# MAC stuff
set(CMAKE_MACOSX_RPATH 1)

# the version number.
set (PARSER_VERSION_MAJOR 1)
set (PARSER_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/parserVersion.h.in" "${PROJECT_BINARY_DIR}/parserVersion.h")

# add the binary tree to the search path for include files so that we will find parserVersion.h
include_directories("${PROJECT_BINARY_DIR}")

# add customized flags
SET(GCC_CUSTOMIZED_COMPILE_FLAGS "-std=c++14")
SET(GCC_CUSTOMIZED_LINK_FLAGS "")

# apply these flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CUSTOMIZED_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_CUSTOMIZED_LINK_FLAGS}")

# add the executable
add_library(stonepine.parser SHARED xerces.cpp)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(OperatingSystem "Mac OS X")

	# add links
	target_link_libraries(stonepine.parser xerces-c logger)

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# add the install targets
install (TARGETS stonepine.parser DESTINATION lib)
install (FILES xerces.h "${PROJECT_BINARY_DIR}/parserVersion.h" DESTINATION include/stonepine/parser)
