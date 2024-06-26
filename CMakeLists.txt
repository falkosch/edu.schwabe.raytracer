cmake_minimum_required(VERSION 3.9)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
#    execute_process(
#            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#            RESULT_VARIABLE GIT_SUBMOD_RESULT
#    )
#    if (NOT GIT_SUBMOD_RESULT EQUAL "0")
#        message(FATAL_ERROR "${GIT_SUBMOD_RESULT}: updating submodules failed, please checkout submodules manually")
#        return()
#    endif ()
#endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/conan/${CMAKE_VS_PLATFORM_NAME}")

project(Raytracer)

include(Catch2Tests)

add_compile_definitions(UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)

if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "")
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif ()

set(CompilerFlags CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    foreach (CompilerFlag ${CompilerFlags})
        string(REGEX REPLACE "-g[0-9]?" "" ${CompilerFlag} "${${CompilerFlag}}")
        string(REGEX REPLACE "-O[0-9]?" "" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach ()

    add_compile_definitions(_WIN32)

    add_compile_options(-g3 -march=native -fopenmp
            -Wall -Wextra -pedantic -Wno-ignored-qualifiers -Wno-unknown-pragmas)

    add_link_options(-g3 -fopenmp)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    foreach (CompilerFlag ${CompilerFlags})
        string(REGEX REPLACE "/[wW][0-9]" "" ${CompilerFlag} "${${CompilerFlag}}")
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach ()

    add_compile_options(/arch:AVX2 /openmp /GS- /fp:fast /permissive- /Zc:twoPhase- /Zp16 /sdl- /Oi
            /W4 /wd4514 /wd4710 /wd4711 /wd4820)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()

foreach (CompilerFlag ${CompilerFlags})
    message(NOTICE "${CompilerFlag}=${${CompilerFlag}}")
endforeach ()

add_subdirectory(sources)
add_subdirectory(tests)

add_custom_target(sources_and_tests DEPENDS sources tests)

add_subdirectory(docs)

add_custom_target(ci DEPENDS sources_and_tests docs reports)
