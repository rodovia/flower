project(WindowCreator LANGUAGES C)

add_library(wc source/unix/window.c source/unix/init.c)
target_include_directories(wc PUBLIC include)
