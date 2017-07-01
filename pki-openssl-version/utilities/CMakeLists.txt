cmake_minimum_required (VERSION 2.6)

# MAC stuff
set(CMAKE_MACOSX_RPATH 1)

# the version number.
set (UTILITIES_VERSION_MAJOR 1)
set (UTILITIES_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/utilitiesVersion.h.in" "${PROJECT_BINARY_DIR}/utilitiesVersion.h")

# add the binary tree to the search path for include files so that we will find utilitiesVersion.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories(../../)

# add customized flags
SET(GCC_CUSTOMIZED_COMPILE_FLAGS "-std=c++14")
SET(GCC_CUSTOMIZED_LINK_FLAGS "")

# apply these flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CUSTOMIZED_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_CUSTOMIZED_LINK_FLAGS}")

# add the source files
add_library(stonepine.utilities SHARED stonepine.utilities.boost.cpp stonepine.utilities.regex.cpp stonepine.utilities.math.cpp binary/base64.cpp binary/binary.cpp genericFromXml.cpp stonepine.utilities.time.cpp utilities.cpp stonepine.utilities.iO.cpp stonepine.utilities.windows.dump.cpp)

# add links
#target_link_libraries(stonepine.utilities stonepine.logger)

# add the install targets
install (TARGETS stonepine.utilities DESTINATION lib)
install (FILES utilitiesVersion.h stonepine.utilities.boost.h genericFromXml.h stonepine.utilities.iO.h stonepine.utilities.math.h stonepine.utilities.regex.h stonepine.utilities.time.h utilities.h stonepine.utilities.windows.dump.h DESTINATION include/stonepine/utilities)
install (FILES binary/base64.h binary/binary.h DESTINATION include/stonepine/utilities/binary)
