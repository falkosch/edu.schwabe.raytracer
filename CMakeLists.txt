cmake_minimum_required(VERSION 3.9)

project(Raytracer)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    string (REGEX REPLACE "/[wW][0-9]" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    string (REGEX REPLACE "/[wW][0-9]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

    add_compile_options(
        /MP
        /W4
        /wd4514
        /wd4710
        /wd4711
        /wd4820
    )
else()
    add_compile_options(
        -Wall
        -Wextra
        -pedantic
        -Wno-ignored-qualifiers
        -Wno-unknown-pragmas
    )
endif()

add_subdirectory(sources)
add_subdirectory(tests)
add_subdirectory(docs)
