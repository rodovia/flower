project(http)

set(LIBHTTP_OPTIONS "")
set(LIBHTTP_SOURCES source/url.cpp source/request.cpp source/socket.cpp source/tls.cpp)
set(LIBHTTP_LIBS "")

option(ENABLE_HTTPS "Enables HTTP over TLS support" YES)

if (ENABLE_HTTPS)
    find_package(GnuTLS REQUIRED)
    set(LIBHTTP_LIBS GnuTLS::GnuTLS ${LIBHTTP_LIBS})
    list(APPEND LIBHTTP_OPTIONS -DHTTP_WANT_TLS)
endif()

add_library(http STATIC ${LIBHTTP_SOURCES})
target_link_libraries(http PUBLIC ${LIBHTTP_LIBS})
target_include_directories(http PUBLIC .)
target_compile_definitions(http PRIVATE ${LIBHTTP_OPTIONS})