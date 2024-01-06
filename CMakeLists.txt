cmake_minimum_required(VERSION 3.25)
project(Flower)

set(CMAKE_CXX_STANDARD 20)

##### External deps #####
add_subdirectory(3/frozen)
##########################

add_subdirectory(http)
add_subdirectory(draw)
add_subdirectory(windowcreator)
add_subdirectory(aux/pangob2d)

add_executable(flower source/main.cpp source/html.cpp source/css/css.cpp
                    source/css/css_color.cpp source/css/css_selector.cpp)
target_link_libraries(flower PUBLIC http draw wc frozen::frozen)

