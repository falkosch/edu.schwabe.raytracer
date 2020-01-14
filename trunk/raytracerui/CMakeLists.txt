project(raytracerui CXX)

add_executable(${PROJECT_NAME}
    "include/Benchmarks.h"
    "include/ManualTests.h"
    "include/MessageLoopBasedUI.h"
    "include/OpenGLWindowsRaytracerUI.h"
    "include/RaytracerUI.h"
    "include/tests/CornellBoxScene.h"
    "include/tests/DragonScene.h"
    "include/tests/ProceduralScene.h"
    "include/tests/TestLightScene.h"
    "include/tests/TestResources.h"
    "include/tests/TestScene1.h"
    "include/tests/TestScene2.h"
    "include/WindowsRaytracerUI.h"

    "src/stdafx.h"

    "src/Benchmarks.cpp"
    "src/main.cpp"
    "src/ManualTests.cpp"
    "src/MessageLoopBasedUI.cpp"
    "src/OpenGLWindowsRaytracerUI.cpp"
    "src/RaytracerUI.cpp"
    "src/tests/CornellBoxScene.cpp"
    "src/tests/DragonScene.cpp"
    "src/tests/TestLightScene.cpp"
    "src/tests/TestResources.cpp"
    "src/tests/TestScene1.cpp"
    "src/tests/TestScene2.cpp"
    "src/WindowsRaytracerUI.cpp"
)

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX "Files"
    REGULAR_EXPRESSION ".+\\.(h|cpp)"
)

target_compile_features(${PROJECT_NAME}
    PUBLIC
        cxx_std_17
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

add_dependencies(${PROJECT_NAME} raytracing vectorization)

find_package(GLEW REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raytracing
        vectorization
        opengl32
        glu32
        GLEW::GLEW
)

#target_link_directories(${PROJECT_NAME}
#    PUBLIC
#        "${OUTPUT_DIRECTORY}"
#)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        "$<$<CONFIG:Debug>:"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<STREQUAL:${CMAKE_VS_PLATFORM_NAME},x64>:"
            "X64"
        ">"
        "$<$<STREQUAL:${CMAKE_VS_PLATFORM_NAME},x86>:"
        ">"
        "WIN32_LEAN_AND_MEAN;"
        "NOMINMAX;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
)

if(MSVC)

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            $<$<CONFIG:Debug>:
                /sdl
            >
            $<$<CONFIG:Release>:
                /GT
                /Gy
                /GS-
                /openmp
            >
            $<$<STREQUAL:${CMAKE_VS_PLATFORM_NAME},x86>:
                /arch:SSE2
            >
            /wd4514
            /wd4710
            /wd4711
            /wd4820
    )

    target_link_options(${PROJECT_NAME}
        PRIVATE
            $<$<CONFIG:Debug>:
            >
            $<$<CONFIG:Release>:
                /OPT:REF
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
    )
endif()
