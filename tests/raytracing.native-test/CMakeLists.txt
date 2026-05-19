project("raytracing.native-test" LANGUAGES CXX VERSION 1.0.0)

set(SOURCES
        src/raytracing/common/Bitmap_test.cpp
        src/raytracing/common/HDRImage_test.cpp
        src/raytracing/common/RGBS_test.cpp
        src/raytracing/geometry/forms/Box_test.cpp
        src/raytracing/geometry/forms/Plane_test.cpp
        src/raytracing/geometry/forms/Sphere_test.cpp
        src/raytracing/geometry/forms/meshes/Mesh_test.cpp
        src/raytracing/geometry/partitioning/Balancer_test.cpp
        src/raytracing/geometry/partitioning/Traverser_test.cpp
        src/raytracing/scene/Camera_test.cpp
        src/raytracing/shading/BRDF_test.cpp
        src/raytracing/shading/BeerLambert_test.cpp
        src/raytracing/shading/EnvironmentShader_test.cpp
        src/raytracing/shading/Fresnel_test.cpp
        src/raytracing/shading/HDRImageShader_test.cpp
        src/raytracing/shading/IntersectionNormalShader_test.cpp
        src/raytracing/shading/Phong_test.cpp
        src/raytracing/shading/brdf/ggx_test.cpp
        src/raytracing/shading/color/primaries_test.cpp
        src/raytracing/shading/color/view_transform_test.cpp
)

set(HEADERS
        include/raytracing_tests.h
        src/stdafx.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" REGULAR_EXPRESSION ".+\\.(h|cpp)")

add_dependencies(${PROJECT_NAME} raytracing)

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{VCInstallDir}/Auxiliary/VS/UnitTest/lib")

target_link_libraries(${PROJECT_NAME} PRIVATE raytracing)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        "$ENV{VCInstallDir}/Auxiliary/VS/UnitTest/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/stdafx.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions)
    endif ()

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
