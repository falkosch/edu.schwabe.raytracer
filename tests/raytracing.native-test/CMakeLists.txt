project("raytracing.native-test" LANGUAGES CXX VERSION 1.0.0)

set(SOURCES
        src/raytracing/geometry/forms/Sphere_test.cpp
        src/raytracing/geometry/forms/Box_test.cpp
        src/raytracing/geometry/forms/Plane_test.cpp
        src/raytracing/geometry/forms/meshes/Mesh_test.cpp
        src/raytracing/geometry/partitioning/Balancer_test.cpp
        src/raytracing/geometry/partitioning/Traverser_test.cpp
        src/raytracing/shading/Fresnel_test.cpp
        src/raytracing/shading/BeerLambert_test.cpp
        src/raytracing/shading/Phong_test.cpp
        src/raytracing/shading/BRDF_test.cpp
        src/raytracing/shading/IntersectionNormalShader_test.cpp
        src/raytracing/scene/Camera_test.cpp
        src/raytracing/common/Bitmap_test.cpp
        src/raytracing/common/HDRImage_test.cpp
)

set(HEADERS
        include/raytracing_tests.h
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

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
