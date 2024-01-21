project(libDraw)

find_package(X11 REQUIRED)
include(../aux/FindCairo.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Pango REQUIRED pangocairo IMPORTED_TARGET)

add_library(draw STATIC source/window.cpp source/text.cpp source/scrollbar.cpp
                        source/layout.cpp source/rectangle.cpp source/navigator.cpp
                        source/paintable/chrome.cpp source/paintable/address_bar.cpp
                        source/layout_table.cpp)
target_link_libraries(draw PUBLIC X11::X11 Cairo::Cairo PkgConfig::Pango wc)
target_include_directories(draw PUBLIC . PUBLIC ../source ../http PRIVATE ../3)