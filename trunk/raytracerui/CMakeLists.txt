project(raytracerui CXX)

# Source groups
set(HEADER_FILES
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
)
source_group("Header files" FILES ${HEADER_FILES})

set(SOURCE_FILES
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
source_group("Source files" FILES ${SOURCE_FILES})

set(ALL_FILES
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# Target
add_executable(${PROJECT_NAME} ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE raytracerui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)

# Output directory
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build/$ENV{PlatformShortName}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/$ENV{PlatformShortName}/$<CONFIG>/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build/$ENV{PlatformShortName}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/$ENV{PlatformShortName}/$<CONFIG>/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()

# Include directories
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vectorization/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../raytracing/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../packages/glew.v140.1.12.0/build/native/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vectorization/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../raytracing/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../packages/glew.v140.1.12.0/build/native/include"
    )
endif()

# Compile definitions
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32_LEAN_AND_MEAN;"
        "NOMINMAX;"
        "X64;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32_LEAN_AND_MEAN;"
        "NOMINMAX;"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

# Compile and link options
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /sdl;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                /GS-;
                /openmp;
                ${DEFAULT_CXX_RUNTIME_LIBRARY}
            >
            /std:c++17;
            /W4;
            /wd4514;
            /wd4710;
            /wd4711;
            /wd4820;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /sdl;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                /GS-;
                /openmp;
                ${DEFAULT_CXX_RUNTIME_LIBRARY}
            >
            /std:c++17;
            /arch:SSE2;
            /W4;
            /wd4514;
            /wd4710;
            /wd4711;
            /wd4820;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

# Dependencies
add_dependencies(${PROJECT_NAME}
    raytracing
    vectorization
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    raytracing
    vectorization
)

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "vectorization;"
        "raytracing;"
        "glu32;"
        "opengl32"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "vectorization;"
        "raytracing;"
        "glu32;"
        "opengl32"
    )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "${OUTPUT_DIRECTORY}"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "${OUTPUT_DIRECTORY}"
    )
endif()
