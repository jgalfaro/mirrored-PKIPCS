cmake_minimum_required (VERSION 2.6)

project (stonepine.database)

# MAC stuff
set(CMAKE_MACOSX_RPATH 1)

# the version number.
set (DATABASE_VERSION_MAJOR 1)
set (DATABASE_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/databaseVersion.h.in" "${PROJECT_BINARY_DIR}/databaseVersion.h")

# add the binary tree to the search path for include files so that we will find databaseVersion.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories(../../)

# add customized flags
SET(GCC_CUSTOMIZED_COMPILE_FLAGS "-std=c++14")
SET(GCC_CUSTOMIZED_LINK_FLAGS "")

# macro definitions
add_definitions(-DDATABASE_PGSQL)

# apply these flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CUSTOMIZED_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_CUSTOMIZED_LINK_FLAGS}")

# add the source files
add_library(stonepine.database SHARED database.cpp odbDatabase.cpp odbDatabaseFromXml.cpp)

# add links
#target_link_libraries(stonepine.database odb odb-pgsql)
#target_link_libraries(stonepine.database databaseClientConfigBinding authorizedCertificatesConfigBinding authenticationConfigBinding)
#target_link_libraries(stonepine.database stonepine.libraries stonepine.network stonepine.logger)

# add the install targets
install (TARGETS stonepine.database DESTINATION lib)
install (FILES database.h odbDatabase.h odbDatabaseFromXml.h odbDatabaseOperations.h databaseVersion.h DESTINATION include/stonepine/database)
