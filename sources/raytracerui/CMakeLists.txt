project(raytracerui CXX)

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

target_compile_features(${PROJECT_NAME}
    PUBLIC
        cxx_std_17
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
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
        $<$<CONFIG:Release>:
            NDEBUG
        >
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        _CONSOLE
)

if(MSVC)

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            $<$<CONFIG:Release>:
                /openmp
            >
            /permissive-
            /Zc:twoPhase-
    )

    target_link_options(${PROJECT_NAME}
        PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
    )
endif()
