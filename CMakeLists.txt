cmake_minimum_required(VERSION 3.0.0)

project(qore-asn1-module VERSION 0.0.3 LANGUAGES CXX)

option(INSTALL_DOCS "Install documentation" OFF)

include(CheckCXXCompilerFlag)

find_package(Qore REQUIRED)
find_package(OpenSSL REQUIRED)

check_cxx_compiler_flag(-fvisibility=hidden HAVE_GCC_VISIBILITY)

set(CMAKE_REQUIRED_INCLUDES ${OPENSSL_INCLUDES})
set(CMAKE_REQUIRED_LIBRARIES ${OPENSSL_SSL_LIBRARY})
check_cxx_source_compiles("
#include <openssl/asn1.h>
int main(void) {
const unsigned char *p;
d2i_ASN1_type_bytes(0, &p, 0, 0);
return 0;
}"
HAVE_OPENSSL_CONST)
unset(CMAKE_REQUIRED_INCLUDES)
unset(CMAKE_REQUIRED_LIBRARIES)

configure_file(${CMAKE_SOURCE_DIR}/cmake/config.h.cmake config.h)

# Check for C++11.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(module_name asn1)
set(CPP_SRC
    src/asn1.cc
    src/QC_ASN1Sequence.cc
    src/QC_ASN1Integer.cc
    src/QC_ASN1ObjectIdentifier.cc
    src/QC_ASN1Boolean.cc
    src/QC_ASN1String.cc
    src/QC_ASN1OctetString.cc
    src/QC_ASN1UTF8String.cc
    src/QC_ASN1BitString.cc
)

add_library(${module_name} SHARED ${CPP_SRC})
target_include_directories(${module_name} PUBLIC ${OPENSSL_INCLUDE_DIR})

qore_binary_module(${module_name} ${PROJECT_VERSION} ${OPENSSL_SSL_LIBRARY})

qore_dist(${PROJECT_VERSION})

qore_config_info()

if(INSTALL_DOCS)
include(GNUInstallDirs)
install(FILES ${CMAKE_SOURCE_DIR}/docs/asn1-module-doc.html
        DESTINATION ${CMAKE_INSTALL_FULL_DOCDIR}/html)
endif()
