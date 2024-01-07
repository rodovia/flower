project(pangob2d)

find_package(PkgConfig REQUIRED)
find_package(Fontconfig REQUIRED)
pkg_check_modules(Pango REQUIRED pango IMPORTED_TARGET)
add_library(pangob2d SHARED source/fontmap.c source/fontfamily.c)
target_link_libraries(pangob2d PRIVATE PkgConfig::Pango 
                                       blend2d 
                                       Fontconfig::Fontconfig)
target_include_directories(pangob2d PUBLIC source)