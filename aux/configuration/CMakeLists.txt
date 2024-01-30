project(Configuration)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Config libconfig++ IMPORTED_TARGET)
add_library(config INTERFACE)
target_link_libraries(config INTERFACE PkgConfig::Config)
target_include_directories(config INTERFACE source)
