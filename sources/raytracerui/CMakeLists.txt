project(raytracerui
    LANGUAGES CXX
    VERSION 1.0.0
)

add_executable(${PROJECT_NAME}
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
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Files"
    REGULAR_EXPRESSION ".+\\.(h|cpp)"
)

add_dependencies(${PROJECT_NAME}
    raytracing
)

find_package(RealGLEW)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        raytracing
        gdi32
        opengl32
        glu32
        RealGLEW::RealGLEW
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        _CONSOLE
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_options(${PROJECT_NAME}
        PRIVATE
            /SUBSYSTEM:CONSOLE
    )

else()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif()
