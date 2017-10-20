cmake_minimum_required (VERSION 2.6)

project (test)

# the version number.
set (TEST_VERSION_MAJOR 3)
set (TEST_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/test.version.h.in" "${PROJECT_BINARY_DIR}/test.version.h")

# add the binary tree to the search path for include files so that we will find test.version.h
include_directories("${PROJECT_BINARY_DIR}")
# add customized flags
SET(GCC_CUSTOMIZED_COMPILE_FLAGS "-std=c++14")
SET(GCC_CUSTOMIZED_LINK_FLAGS "")

# apply these flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CUSTOMIZED_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_CUSTOMIZED_LINK_FLAGS}")

# add the source files
add_executable(pcsTest main.cpp)

# add links

# own open source built libraries

find_library(TEPLA_LIBRARY tepla HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${TEPLA_LIBRARY})

find_library(P11_LIBRARY p11 HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${P11_LIBRARY})

find_library(ODB_LIBRARY odb HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${ODB_LIBRARY})

find_library(ODBBOOST_LIBRARY odb-boost HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${ODBBOOST_LIBRARY})

find_library(ODBPGSQL_LIBRARY odb-pgsql HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${ODBPGSQL_LIBRARY})

find_library(PQ_LIBRARY pq HINTS /usr/lib/x86_64-linux-gnu)
target_link_libraries(pcsTest PUBLIC ${PQ_LIBRARY})

# system libraries

find_library(UUID_LIBRARY uuid HINTS /usr/lib/x86_64-linux-gnu)
target_link_libraries(pcsTest PUBLIC ${UUID_LIBRARY})

find_library(XERCESC_LIBRARY xerces-c HINTS /usr/lib/x86_64-linux-gnu)
target_link_libraries(pcsTest PUBLIC ${XERCESC_LIBRARY})

find_library(GMP_LIBRARY gmp HINTS /usr/lib/x86_64-linux-gnu)
target_link_libraries(pcsTest PUBLIC ${GMP_LIBRARY})

find_library(CRYPTO_LIBRARY crypto HINTS /usr/lib/x86_64-linux-gnu)
target_link_libraries(pcsTest PUBLIC ${CRYPTO_LIBRARY})

find_library(SSL_LIBRARY ssl HINTS /usr/lib/x86_64-linux-gnu)
target_link_libraries(pcsTest PUBLIC ${SSL_LIBRARY})

find_library(EVENT_LIBRARY event HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${EVENT_LIBRARY})

find_library(EVENTOPENSSL_LIBRARY event_openssl HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${EVENTOPENSSL_LIBRARY})

# stonepine libraries

find_library(STONEPINEOBFUSCATE_LIBRARY stonepine.obfuscate HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEOBFUSCATE_LIBRARY})

find_library(STONEPINELOGGER_LIBRARY stonepine.logger HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINELOGGER_LIBRARY})

find_library(STONEPINEUTILITIES_LIBRARY stonepine.utilities HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEUTILITIES_LIBRARY})

find_library(STONEPINEUTILITIESAPPLICATION_LIBRARY stonepine.utilities.application HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEUTILITIESAPPLICATION_LIBRARY})

find_library(STONEPINEUTILITIESCOMMANDLINEOPTIONS_LIBRARY stonepine.utilities.commandLineOptions HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEUTILITIESCOMMANDLINEOPTIONS_LIBRARY})

find_library(STONEPINEUTILITIESXML_LIBRARY stonepine.utilities.xml HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEUTILITIESXML_LIBRARY})

find_library(STONEPINEUTILITIESXMLCLIENT_LIBRARY stonepine.utilities.xml.client HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEUTILITIESXMLCLIENT_LIBRARY})

find_library(STONEPINEUTILITIESXMLSERVER_LIBRARY stonepine.utilities.xml.server HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEUTILITIESXMLSERVER_LIBRARY})

find_library(STONEPINEUTILITIESXMLDATABASE_LIBRARY stonepine.utilities.xml.database HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEUTILITIESXMLDATABASE_LIBRARY})

find_library(STONEPINEUTILITIESXMLAPPLICATION_LIBRARY stonepine.utilities.xml.application HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEUTILITIESXMLAPPLICATION_LIBRARY})

find_library(STONEPINELIBRARIES_LIBRARY stonepine.libraries HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINELIBRARIES_LIBRARY})

find_library(STONEPINELIBRARIESPCSSERVER_LIBRARY stonepine.libraries.pcsStore.server HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINELIBRARIESPCSSERVER_LIBRARY})

find_library(STONEPINESECURITY_LIBRARY stonepine.security HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINESECURITY_LIBRARY})

find_library(STONEPINENETWORK_LIBRARY stonepine.network HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINENETWORK_LIBRARY})

find_library(STONEPINENETWORKCLIENT_LIBRARY stonepine.network.client HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINENETWORKCLIENT_LIBRARY})

find_library(STONEPINENETWORKSERVER_LIBRARY stonepine.network.server HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINENETWORKSERVER_LIBRARY})

find_library(STONEPINEDATABASE_LIBRARY stonepine.database HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEDATABASE_LIBRARY})

find_library(STONEPINEBINDINGXMLSERVER_LIBRARY stonepine.binding.xml.server HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLSERVER_LIBRARY})

find_library(STONEPINEBINDINGXMLCLIENT_LIBRARY stonepine.binding.xml.client HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLCLIENT_LIBRARY})

find_library(STONEPINEBINDINGXMLAPPLICATION_LIBRARY stonepine.binding.xml.application HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLAPPLICATION_LIBRARY})

find_library(STONEPINEBINDINGXMLX509CERTIFICATE_LIBRARY stonepine.binding.xml.x509Certificate HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLX509CERTIFICATE_LIBRARY})

find_library(STONEPINEBINDINGXMLPCSSTORE_LIBRARY stonepine.binding.xml.pcsStore HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLPCSSTORE_LIBRARY})

find_library(STONEPINEBINDINGXMLDATABASECLIENT_LIBRARY stonepine.binding.xml.databaseClient HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLDATABASECLIENT_LIBRARY})

find_library(STONEPINEBINDINGXMLAUTHORIZEDCERTIFICATE_LIBRARY stonepine.binding.xml.authorizedCertificate HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLAUTHORIZEDCERTIFICATE_LIBRARY})

find_library(STONEPINEBINDINGXMLAUTHENTICATION_LIBRARY stonepine.binding.xml.authentication HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLAUTHENTICATION_LIBRARY})

find_library(STONEPINEBINDINGXMLCERTIFICATE_LIBRARY stonepine.binding.xml.certificate HINTS /usr/local/lib)
target_link_libraries(pcsTest PUBLIC ${STONEPINEBINDINGXMLCERTIFICATE_LIBRARY})

# add the install targets
install (TARGETS pcsTest DESTINATION bin)
install (FILES test.version.h DESTINATION bin)
