cmake_minimum_required (VERSION 2.6)

project (stonepine.certificateStoreServer)

# MAC stuff
set(CMAKE_MACOSX_RPATH 1)

# the version number.
set (CERTIFICATESTORESERVER_VERSION_MAJOR 1)
set (CERTIFICATESTORESERVER_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/certificateStoreServerVersion.h.in" "${PROJECT_BINARY_DIR}/certificateStoreServerVersion.h")

# add the binary tree to the search path for include files so that we will find certificateStoreServerVersion.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories(../../../)
include_directories(../../../../tesla/server/)

# add customized flags
SET(GCC_CUSTOMIZED_COMPILE_FLAGS "-std=c++14")
SET(GCC_CUSTOMIZED_LINK_FLAGS "")

# macro definitions
add_definitions(-DDATABASE_PGSQL)

# apply these flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CUSTOMIZED_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_CUSTOMIZED_LINK_FLAGS}")

# add the source files
add_library(stonepine.certificateStoreServer SHARED certificateStoreServer.cpp certificateStoreServerFromXml.cpp)

# add links
#target_link_libraries(stonepine.certificateStoreServer odb odb-pgsql odb-boost event ssl)
#target_link_libraries(stonepine.certificateStoreServer certificateStoreConfigBinding clientConfigBinding x509CertificateBinding certificateStoreBinding)
#target_link_libraries(stonepine.certificateStoreServer certificateStoreDatabase)
#target_link_libraries(stonepine.certificateStoreServer stonepine.libraries stonepine.network stonepine.network.client stonepine.network.server stonepine.utilities stonepine.database stonepine.security stonepine.logger)

# add the install targets
install (TARGETS stonepine.certificateStoreServer DESTINATION lib)
install (FILES certificateStoreServer.h certificateStoreServerFromXml.h certificateStoreServerVersion.h DESTINATION include/stonepine/software/certificateStoreServer)
