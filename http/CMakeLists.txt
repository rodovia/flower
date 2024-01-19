project(http)

set(LIBHTTP_OPTIONS "")
set(LIBHTTP_SOURCES source/url.cpp source/socket.cpp source/tls.cpp
                    source/session.cpp source/codec.cpp source/session_manager.cpp)
set(LIBHTTP_LIBS "")

option(ENABLE_HTTPS "Enables HTTP over TLS support" YES)

if (ENABLE_HTTPS)
    find_package(GnuTLS REQUIRED)
    set(LIBHTTP_LIBS GnuTLS::GnuTLS ${LIBHTTP_LIBS})
    list(APPEND LIBHTTP_OPTIONS -DHTTP_WANT_TLS)
endif()

find_package(Threads REQUIRED)

add_library(http STATIC ${LIBHTTP_SOURCES})
target_link_libraries(http PUBLIC ${LIBHTTP_LIBS} Threads::Threads)
target_include_directories(http PUBLIC .)
target_compile_definitions(http PRIVATE ${LIBHTTP_OPTIONS})