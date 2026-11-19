cmake_minimum_required(VERSION 4.0...4.3 FATAL_ERROR)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Raytracer)

set(CompilerFlags CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# No C++20 modules in use: skip the per-source module dependency scan CMake enables for C++20+ targets (CMP0155).
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Match the C runtime to the vcpkg triplet (MSVC ABI only): static CRT for "*-static" triplets and builds without
# vcpkg, DLL CRT otherwise (e.g. x64-windows, x64-windows-static-md). Mixing CRTs across libraries is unsupported.
if (NOT DEFINED VCPKG_TARGET_TRIPLET OR VCPKG_TARGET_TRIPLET MATCHES "-static$")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else ()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif ()

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

    # Warning level (CMP0092), runtime library (CMP0091), debug info format (CMP0141) and runtime checks (CMP0184)
    # are no longer part of the default flags; only the exception handling flag still needs stripping.
    foreach (CompilerFlag ${CompilerFlags})
        string(REGEX REPLACE "/EH[ascr-]+" "" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach ()

    # CMake (up to at least 4.4) maps CXX_STANDARD 23 to /std:c++latest (C++26 working draft) for MSVC.
    # MSVC Build Tools 14.52 (VS 2026 18.12) provides a stable /std:c++23 (/std:c++23preview is deprecated).
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.52)
        set(CMAKE_CXX23_STANDARD_COMPILE_OPTION "/std:c++23")
        set(CMAKE_CXX23_EXTENSION_COMPILE_OPTION "/std:c++23")
    endif ()

    add_compile_options(/arch:AVX2)
    #add_compile_options(/arch:SSE4.2)
    add_compile_options(/permissive- /Zc:__cplusplus /Zc:preprocessor /W4 /wd4514 /wd4710 /wd4711 /wd4820 /wd4068 /wd4530)
    add_compile_options(/openmp /GS- /fp:fast /Zp16 /sdl- /Oi /Gy)
    add_compile_options($<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${COVERAGE}>>>:/Oy>)
    #add_compile_options($<$<CONFIG:Release>:/d2cgsummary>)
    add_compile_options($<$<CONFIG:Release>:/d2SSAOptimizer->)

    # LTCG: /GL when compiling, /LTCG for link.exe and lib.exe
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

    add_link_options(
            $<$<CONFIG:Release>:/OPT:REF>
            $<$<CONFIG:Release>:/OPT:ICF>
            $<$<CONFIG:Debug>:/Debug>
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        # clang-cl (MSVC-compatible frontend)
        foreach (CompilerFlag ${CompilerFlags})
            string(REGEX REPLACE "/EH[ascr-]+" "" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach ()

        add_compile_options(/W4 /wd4514 /wd4710 /wd4711 /wd4820 /wd4068 /wd4530)
        add_compile_options(/arch:AVX2 /openmp /GS- /Zp16 /sdl-)
        add_compile_options(/clang:-ffp-contract=fast /clang:-fassociative-math /clang:-freciprocal-math /clang:-fno-signed-zeros)

        # LTO: clang-cl ignores /GL; CMake uses ThinLTO (-flto=thin) and links with lld-link instead
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

        link_libraries(libomp)

        add_link_options(
                $<$<CONFIG:Release>:/OPT:REF>
                $<$<CONFIG:Release>:/OPT:ICF>
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
