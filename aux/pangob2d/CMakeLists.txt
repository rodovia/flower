project(pangob2d)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Pango REQUIRED pango IMPORTED_TARGET)
add_library(pangob2d SHARED source/draw.cpp)
target_link_libraries(pangob2d PRIVATE PkgConfig::Pango)
