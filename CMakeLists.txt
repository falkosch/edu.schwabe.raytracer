cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Raytracer)

set(CompilerFlags CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "")
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif ()

add_compile_definitions(UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
add_compile_definitions(
        $<$<STREQUAL:${CMAKE_VS_PLATFORM_NAME},x64>:X64>
        $<$<STREQUAL:${CMAKE_VS_PLATFORM_NAME},x86>:X86>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Debug>:DEBUG>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    foreach (CompilerFlag ${CompilerFlags})
        string(REGEX REPLACE "-g[0-9]?" "" ${CompilerFlag} "${${CompilerFlag}}")
        string(REGEX REPLACE "-O[0-9]?" "" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach ()

    add_compile_definitions(_WIN32)
    add_compile_options(-Wall -Wextra -pedantic -Wno-ignored-qualifiers -Wno-unknown-pragmas)
    add_compile_options(-g3 -march=native -fopenmp -fno-exceptions -ffp-contract=fast -fassociative-math -freciprocal-math -fno-signed-zeros)

    add_link_options(-g3 -fopenmp)

    add_compile_options(
            $<$<AND:$<CONFIG:Release>,$<BOOL:${COVERAGE}>>:-O0>
            $<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${COVERAGE}>>>:-O3>
            $<$<AND:$<CONFIG:Debug>,$<BOOL:${COVERAGE}>>:-O0>
            $<$<AND:$<CONFIG:Debug>,$<NOT:$<BOOL:${COVERAGE}>>>:-Og>
    )

    add_link_options(
            $<$<AND:$<CONFIG:Release>,$<BOOL:${COVERAGE}>>:-O0>
            $<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${COVERAGE}>>>:-O3>
            $<$<AND:$<CONFIG:Debug>,$<BOOL:${COVERAGE}>>:-O0>
            $<$<AND:$<CONFIG:Debug>,$<NOT:$<BOOL:${COVERAGE}>>>:-Og>
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    foreach (CompilerFlag ${CompilerFlags})
        string(REGEX REPLACE "/[wW][0-9]" "" ${CompilerFlag} "${${CompilerFlag}}")
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        string(REGEX REPLACE "/EH[ascr-]+" "" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach ()

    add_compile_options(/arch:AVX2)
    #add_compile_options(/arch:SSE4.2)
    add_compile_options(/permissive- /W4 /wd4514 /wd4710 /wd4711 /wd4820 /wd4068 /wd4530)
    add_compile_options(/openmp /GS- /fp:fast /Zp16 /sdl- /Oi /Gy)
    add_compile_options($<$<CONFIG:Release>:/O2>)
    add_compile_options($<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${COVERAGE}>>>:/Oy>)
    add_compile_options($<$<CONFIG:Release>:/GL>)
    #add_compile_options($<$<CONFIG:Release>:/d2cgsummary>)
    add_compile_options($<$<CONFIG:Release>:/d2SSAOptimizer->)

    add_link_options(
            $<$<CONFIG:Release>:/OPT:REF>
            $<$<CONFIG:Release>:/OPT:ICF>
            $<$<CONFIG:Release>:/LTCG>
            $<$<CONFIG:Debug>:/Debug>
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        # clang-cl (MSVC-compatible frontend)
        foreach (CompilerFlag ${CompilerFlags})
            string(REGEX REPLACE "/[wW][0-9]" "" ${CompilerFlag} "${${CompilerFlag}}")
            string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
            string(REGEX REPLACE "/EH[ascr-]+" "" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach ()

        add_compile_options(/W4 /wd4514 /wd4710 /wd4711 /wd4820 /wd4068 /wd4530)
        add_compile_options(/arch:AVX2 /openmp /GS- /Zp16 /sdl-)
        add_compile_options(/clang:-ffp-contract=fast /clang:-fassociative-math /clang:-freciprocal-math /clang:-fno-signed-zeros)
        add_compile_options($<$<CONFIG:Release>:/O2>)
        add_compile_options($<$<CONFIG:Release>:/GL>)
        add_compile_options($<$<CONFIG:Release>:/d2SSAOptimizer->)

        link_libraries(libomp)

        add_link_options(
                $<$<CONFIG:Release>:/OPT:REF>
                $<$<CONFIG:Release>:/OPT:ICF>
                $<$<CONFIG:Release>:/LTCG>
                $<$<CONFIG:Debug>:/Debug>
        )
    else ()
        # Regular clang (GCC-compatible frontend)
        foreach (CompilerFlag ${CompilerFlags})
            string(REGEX REPLACE "-g[0-9]?" "" ${CompilerFlag} "${${CompilerFlag}}")
            string(REGEX REPLACE "-O[0-9]?" "" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach ()

        add_compile_options(-Wall -Wextra -pedantic -Wno-ignored-qualifiers -Wno-unknown-pragmas)
        add_compile_options(-march=native -fopenmp -fno-exceptions -ffp-contract=fast -fassociative-math -freciprocal-math -fno-signed-zeros)

        add_link_options(-fopenmp)

        add_compile_options(
                $<$<CONFIG:Release>:-O3>
                $<$<CONFIG:Debug>:-Og>
        )

        add_link_options(
                $<$<CONFIG:Release>:-O3>
                $<$<CONFIG:Debug>:-Og>
        )
    endif ()

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()


add_subdirectory(sources)
add_subdirectory(tests)
