#----------------------------------------------------------------
# Generated CMake target import file for configuration "DEBUG".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wolfssl::wolfssl" for configuration "DEBUG"
set_property(TARGET wolfssl::wolfssl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(wolfssl::wolfssl PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libwolfssl.so.35.5.1"
  IMPORTED_SONAME_DEBUG "libwolfssl.so.35"
  )

list(APPEND _cmake_import_check_targets wolfssl::wolfssl )
list(APPEND _cmake_import_check_files_for_wolfssl::wolfssl "${_IMPORT_PREFIX}/lib/libwolfssl.so.35.5.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
