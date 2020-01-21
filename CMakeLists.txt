cmake_minimum_required(VERSION 3.9)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(Raytracer)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_compile_definitions(
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_definitions(
        _WIN32
    )

    add_compile_options(
        -Wall
        -Wextra
        -pedantic
        -Wno-ignored-qualifiers
        -Wno-unknown-pragmas
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
    )
    foreach(CompilerFlag ${CompilerFlags})
        string(REGEX REPLACE "/[wW][0-9]" "" ${CompilerFlag} "${${CompilerFlag}}")
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    add_compile_options(
        /permissive-
        /Zc:twoPhase-
        /fp:fast
        /GS-
        /W4
        /wd4514
        /wd4710
        /wd4711
        /wd4820
    )

else()
    message(FATAL "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif()

add_subdirectory(sources)
add_subdirectory(tests)
add_subdirectory(docs)
