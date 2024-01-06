# Install script for directory: /home/miguelrodovia/dev/flower/3/wolfssl

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "DEBUG")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwolfssl.so.35.5.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwolfssl.so.35"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/miguelrodovia/dev/flower/build/http/wolfssl/libwolfssl.so.35.5.1"
    "/home/miguelrodovia/dev/flower/build/http/wolfssl/libwolfssl.so.35"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwolfssl.so.35.5.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwolfssl.so.35"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/llvm-strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/miguelrodovia/dev/flower/build/http/wolfssl/libwolfssl.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/wolfssl" TYPE DIRECTORY FILES "/home/miguelrodovia/dev/flower/build/http/wolfssl/wolfssl/" FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "(internal.h|cyassl/ctaocrypt/port|wolfssl/wolfcrypt/port/nrf51.h|wolfssl/wolfcrypt/port/arm|wolfssl/wolfcrypt/port/cypress|wolfssl/wolfcrypt/port/Espressif|wolfssl/wolfcrypt/port/iotsafe|wolfssl/wolfcrypt/port/nxp|wolfssl/wolfcrypt/port/pic|wolfssl/wolfcrypt/port/Renesas|wolfssl/wolfcrypt/port/silabs|wolfssl/wolfcrypt/port/st|wolfssl/wolfcrypt/port/ti|wolfssl/wolfcrypt/port/xilinx|wolfssl/wolfcrypt/port/atmel|wolfssl/wolfcrypt/port/af_alg|wolfssl/wolfcrypt/port/kcapi|wolfssl/wolfcrypt/port/devcrypto|wolfssl/wolfcrypt/async.h|wolfssl/wolfcrypt/wc_pkcs11.h|wolfssl/wolfcrypt/pkcs11.h|wolfssl/wolfcrypt/port/cavium|wolfssl/wolfcrypt/port/intel|wolfssl/wolfcrypt/sp.h|wolfssl/wolfcrypt/selftest.h|wolfssl/wolfcrypt/fips.h|wolfssl/wolfcrypt/port/caam)" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cyassl" TYPE DIRECTORY FILES "/home/miguelrodovia/dev/flower/build/http/wolfssl/cyassl/" FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "(internal.h|cyassl/ctaocrypt/port|wolfssl/wolfcrypt/port/nrf51.h|wolfssl/wolfcrypt/port/arm|wolfssl/wolfcrypt/port/cypress|wolfssl/wolfcrypt/port/Espressif|wolfssl/wolfcrypt/port/iotsafe|wolfssl/wolfcrypt/port/nxp|wolfssl/wolfcrypt/port/pic|wolfssl/wolfcrypt/port/Renesas|wolfssl/wolfcrypt/port/silabs|wolfssl/wolfcrypt/port/st|wolfssl/wolfcrypt/port/ti|wolfssl/wolfcrypt/port/xilinx|wolfssl/wolfcrypt/port/atmel|wolfssl/wolfcrypt/port/af_alg|wolfssl/wolfcrypt/port/kcapi|wolfssl/wolfcrypt/port/devcrypto|wolfssl/wolfcrypt/async.h|wolfssl/wolfcrypt/wc_pkcs11.h|wolfssl/wolfcrypt/pkcs11.h|wolfssl/wolfcrypt/port/cavium|wolfssl/wolfcrypt/port/intel|wolfssl/wolfcrypt/sp.h|wolfssl/wolfcrypt/selftest.h|wolfssl/wolfcrypt/fips.h|wolfssl/wolfcrypt/port/caam)" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/wolfssl" TYPE DIRECTORY FILES "/home/miguelrodovia/dev/flower/3/wolfssl/wolfssl/" FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "(internal.h|cyassl/ctaocrypt/port|wolfssl/wolfcrypt/port/nrf51.h|wolfssl/wolfcrypt/port/arm|wolfssl/wolfcrypt/port/cypress|wolfssl/wolfcrypt/port/Espressif|wolfssl/wolfcrypt/port/iotsafe|wolfssl/wolfcrypt/port/nxp|wolfssl/wolfcrypt/port/pic|wolfssl/wolfcrypt/port/Renesas|wolfssl/wolfcrypt/port/silabs|wolfssl/wolfcrypt/port/st|wolfssl/wolfcrypt/port/ti|wolfssl/wolfcrypt/port/xilinx|wolfssl/wolfcrypt/port/atmel|wolfssl/wolfcrypt/port/af_alg|wolfssl/wolfcrypt/port/kcapi|wolfssl/wolfcrypt/port/devcrypto|wolfssl/wolfcrypt/async.h|wolfssl/wolfcrypt/wc_pkcs11.h|wolfssl/wolfcrypt/pkcs11.h|wolfssl/wolfcrypt/port/cavium|wolfssl/wolfcrypt/port/intel|wolfssl/wolfcrypt/sp.h|wolfssl/wolfcrypt/selftest.h|wolfssl/wolfcrypt/fips.h|wolfssl/wolfcrypt/port/caam)" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cyassl" TYPE DIRECTORY FILES "/home/miguelrodovia/dev/flower/3/wolfssl/cyassl/" FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "(internal.h|cyassl/ctaocrypt/port|wolfssl/wolfcrypt/port/nrf51.h|wolfssl/wolfcrypt/port/arm|wolfssl/wolfcrypt/port/cypress|wolfssl/wolfcrypt/port/Espressif|wolfssl/wolfcrypt/port/iotsafe|wolfssl/wolfcrypt/port/nxp|wolfssl/wolfcrypt/port/pic|wolfssl/wolfcrypt/port/Renesas|wolfssl/wolfcrypt/port/silabs|wolfssl/wolfcrypt/port/st|wolfssl/wolfcrypt/port/ti|wolfssl/wolfcrypt/port/xilinx|wolfssl/wolfcrypt/port/atmel|wolfssl/wolfcrypt/port/af_alg|wolfssl/wolfcrypt/port/kcapi|wolfssl/wolfcrypt/port/devcrypto|wolfssl/wolfcrypt/async.h|wolfssl/wolfcrypt/wc_pkcs11.h|wolfssl/wolfcrypt/pkcs11.h|wolfssl/wolfcrypt/port/cavium|wolfssl/wolfcrypt/port/intel|wolfssl/wolfcrypt/sp.h|wolfssl/wolfcrypt/selftest.h|wolfssl/wolfcrypt/fips.h|wolfssl/wolfcrypt/port/caam)" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/wolfssl/example" TYPE FILE FILES
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/echoserver/echoserver.c"
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/sctp/sctp-server.c"
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/sctp/sctp-client-dtls.c"
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/sctp/sctp-client.c"
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/sctp/sctp-server-dtls.c"
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/echoclient/echoclient.c"
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/server/server.c"
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/benchmark/tls_bench.c"
    "/home/miguelrodovia/dev/flower/3/wolfssl/examples/client/client.c"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/wolfssl" TYPE FILE FILES
    "/home/miguelrodovia/dev/flower/3/wolfssl/doc/README.txt"
    "/home/miguelrodovia/dev/flower/3/wolfssl/certs/taoCert.txt"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/wolfssl/wolfssl-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/wolfssl/wolfssl-targets.cmake"
         "/home/miguelrodovia/dev/flower/build/http/wolfssl/CMakeFiles/Export/41263289dc9ec8a87d5b2cd96b0484fa/wolfssl-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/wolfssl/wolfssl-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/wolfssl/wolfssl-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/wolfssl" TYPE FILE FILES "/home/miguelrodovia/dev/flower/build/http/wolfssl/CMakeFiles/Export/41263289dc9ec8a87d5b2cd96b0484fa/wolfssl-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/wolfssl" TYPE FILE FILES "/home/miguelrodovia/dev/flower/build/http/wolfssl/CMakeFiles/Export/41263289dc9ec8a87d5b2cd96b0484fa/wolfssl-targets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/miguelrodovia/dev/flower/build/http/wolfssl/support/wolfssl.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/wolfssl" TYPE FILE FILES
    "/home/miguelrodovia/dev/flower/build/http/wolfssl/wolfssl-config.cmake"
    "/home/miguelrodovia/dev/flower/build/http/wolfssl/wolfssl-config-version.cmake"
    )
endif()

