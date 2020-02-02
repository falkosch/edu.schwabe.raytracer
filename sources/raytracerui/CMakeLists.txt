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
    vectorization
)

find_package(GLEW REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raytracing
        vectorization
        gdi32
        opengl32
        glu32
        GLEW::GLEW
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        _CONSOLE
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            stdc++
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
    )

    target_link_options(${PROJECT_NAME}
        PRIVATE
            /SUBSYSTEM:CONSOLE
    )

else()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif()
