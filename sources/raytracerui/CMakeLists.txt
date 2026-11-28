project(raytracerui LANGUAGES CXX VERSION 1.0.0)

set(SOURCES
        src/MessageLoopBasedUI.cpp
        src/OpenGLWindowsRaytracerUI.cpp
        src/RaytracerUI.cpp
        src/WindowsRaytracerUI.cpp
        src/main.cpp
        src/tests/CornellBoxScene.cpp
        src/tests/FormsScene.cpp
        src/tests/LightingScene.cpp
        src/tests/MaterialsScene.cpp
        src/tests/MeshesScene.cpp
        src/tests/SceneCatalog.cpp
        src/tests/SceneMaterial.cpp
        src/tests/SceneObjects.cpp
        src/tests/ShadingScene.cpp
        src/tests/ShowcaseScene.cpp
        src/tests/TestResources.cpp
)

set(HEADERS
        include/ButtonStates.h
        include/DragTypes.h
        include/InteractionMode.h
        include/MessageLoopBasedUI.h
        include/MouseButtons.h
        include/OpenGLWindowsRaytracerUI.h
        include/RaytracerUI.h
        include/WindowsRaytracerUI.h
        include/tests/CornellBoxScene.h
        include/tests/FormsScene.h
        include/tests/LightingScene.h
        include/tests/MaterialsScene.h
        include/tests/MeshesScene.h
        include/tests/Placement.h
        include/tests/ProceduralScene.h
        include/tests/SceneCatalog.h
        include/tests/SceneEntry.h
        include/tests/SceneMaterial.h
        include/tests/SceneObjects.h
        include/tests/ShadingScene.h
        include/tests/ShowcaseScene.h
        include/tests/TestResources.h
        src/stdafx.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
copy_openmp_runtime(${PROJECT_NAME})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" REGULAR_EXPRESSION ".+\\.(h|cpp)")

add_dependencies(${PROJECT_NAME} raytracing)

find_package(GLEW REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        raytracing
        gdi32
        opengl32
        glu32
        comdlg32
        GLEW::GLEW
        imgui::imgui
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_precompile_headers(${PROJECT_NAME} PRIVATE src/stdafx.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)
    endif ()

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
