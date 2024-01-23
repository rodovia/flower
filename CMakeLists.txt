cmake_minimum_required(VERSION 3.25)
project(Flower)

set(CMAKE_CXX_STANDARD 20)

option(FLOWER_BUILD_TESTS "Build tests alongside flower binary" NO)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall
                        -Wno-reorder-ctor
                        -Wno-embedded-directive # Embedding a directive within 
                                                # macro arguments has undefined behaviour
                        -Wno-unused-variable
                        -Wno-sometimes-uninitialized # Clang is dumb enough to not notice
                                                     # the if statement
                        -Wstrict-prototypes # Function declaration without a prototype 
                                            # is deprecated in all versions of C
                        -Wno-sign-compare)
endif()


##### External deps #####
add_subdirectory(3/frozen)
##########################

add_subdirectory(http)
add_subdirectory(draw)
add_subdirectory(windowcreator)

if (FLOWER_BUILD_TESTS)
    #add_subdirectory(test/layout)
endif()

add_executable(flower source/main.cpp source/html.cpp source/css/css.cpp
                    source/css/css_color.cpp source/css/css_selector.cpp)
target_link_libraries(flower PUBLIC http draw wc frozen::frozen)

