project(raytracing CXX)

################################################################################
# Source groups
################################################################################
set(HEADER_FILES
    "include/raytracing.h"
    "include/raytracing/BRDFParameters.h"
    "include/raytracing/common/Bitmap.h"
    "include/raytracing/common/HDRImage.h"
    "include/raytracing/common/Image.h"
    "include/raytracing/common/noise/NoiseGenerator.h"
    "include/raytracing/common/noise/PerlinNoiseGenerator.h"
    "include/raytracing/common/noise/SimplexNoiseGenerator.h"
    "include/raytracing/common/RefractionIndices.h"
    "include/raytracing/common/StatisticsCookie.h"
    "include/raytracing/common/Tools.h"
    "include/raytracing/geometry/FacetIntersection.h"
    "include/raytracing/geometry/forms/Box.h"
    "include/raytracing/geometry/forms/Form.h"
    "include/raytracing/geometry/forms/meshes/facets.h"
    "include/raytracing/geometry/forms/meshes/Mesh.h"
    "include/raytracing/geometry/forms/meshes/MeshGeometryNode.h"
    "include/raytracing/geometry/forms/meshes/textureCoordinates.h"
    "include/raytracing/geometry/forms/meshes/vertices.h"
    "include/raytracing/geometry/forms/Plane.h"
    "include/raytracing/geometry/forms/Sphere.h"
    "include/raytracing/geometry/Intersectable.h"
    "include/raytracing/geometry/ObjectGeometry.h"
    "include/raytracing/geometry/partitioning/GeometryNode.h"
    "include/raytracing/geometry/partitioning/GeometryNodesTraverser.h"
    "include/raytracing/geometry/partitioning/KDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/KDTreeBounding.h"
    "include/raytracing/geometry/partitioning/KDTreeBuildParameters.h"
    "include/raytracing/geometry/partitioning/KDTreeNode.h"
    "include/raytracing/geometry/partitioning/KDTreeNodeChilds.h"
    "include/raytracing/geometry/partitioning/KDTreePlane.h"
    "include/raytracing/geometry/partitioning/KDTreeRoot.h"
    "include/raytracing/geometry/partitioning/KDTreeTraverser.h"
    "include/raytracing/geometry/partitioning/treebalancer/ArithmeticMeanKDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/treebalancer/FixedIterationsSAHKDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/treebalancer/MaxAxisKDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/treebalancer/RotatingAxisKDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/treebalancer/SAHKDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/treebalancer/SamplingKDTreeBalancer.h"
    "include/raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h"
    "include/raytracing/geometry/partitioning/treetraverser/VoxelizationKDTreeTraverser.h"
    "include/raytracing/geometry/SceneGeometry.h"
    "include/raytracing/geometry/SceneIntersection.h"
    "include/raytracing/geometry/TransformInfo.h"
    "include/raytracing/IlluminatedIntersection.h"
    "include/raytracing/RaytraceConfiguration.h"
    "include/raytracing/RaytraceObserver.h"
    "include/raytracing/RaytraceOutput.h"
    "include/raytracing/RaytraceParameters.h"
    "include/raytracing/Raytracer.h"
    "include/raytracing/RaytracerCache.h"
    "include/raytracing/RaytracerPackets.h"
    "include/raytracing/scene/Camera.h"
    "include/raytracing/scene/Resources.h"
    "include/raytracing/scene/Scene.h"
    "include/raytracing/scene/SceneObject.h"
    "include/raytracing/shading/LightInfo.h"
    "include/raytracing/shading/LightShading.h"
    "include/raytracing/shading/ObjectShader.h"
    "include/raytracing/shading/PerLightShadowCache.h"
    "include/raytracing/shading/Raytrace.h"
    "include/raytracing/shading/RaytraceBaseParameters.h"
    "include/raytracing/shading/SceneShader.h"
    "include/raytracing/shading/SceneShaderContainment.h"
    "include/raytracing/shading/Shader.h"
    "include/raytracing/shading/shaders/ConcatenatingShader.h"
    "include/raytracing/shading/shaders/ConstShader.h"
    "include/raytracing/shading/shaders/EnvironmentShader.h"
    "include/raytracing/shading/shaders/HDRImageShader.h"
    "include/raytracing/shading/shaders/IntersectionNormalShader.h"
    "include/raytracing/shading/shaders/MultiplyByConstShader.h"
    "include/raytracing/shading/shaders/NoiseGeneratorMap.h"
    "include/raytracing/shading/SurfaceShading.h"
    "include/raytracing/shading/TextureTransformInfo.h"
    "src/stdafx.h"
)
source_group("Header files" FILES ${HEADER_FILES})

set(SOURCE_FILES
    "src/raytracing/BRDFParameters.cpp"
    "src/raytracing/common/Bitmap.cpp"
    "src/raytracing/common/HDRImage.cpp"
    "src/raytracing/common/noise/PerlinNoiseGenerator.cpp"
    "src/raytracing/common/noise/SimplexNoiseGenerator.cpp"
    "src/raytracing/common/RefractionIndices.cpp"
    "src/raytracing/common/StatisticsCookie.cpp"
    "src/raytracing/common/Tools.cpp"
    "src/raytracing/geometry/FacetIntersection.cpp"
    "src/raytracing/geometry/forms/Box.cpp"
    "src/raytracing/geometry/forms/Form.cpp"
    "src/raytracing/geometry/forms/meshes/facets.cpp"
    "src/raytracing/geometry/forms/meshes/Mesh.cpp"
    "src/raytracing/geometry/forms/meshes/MeshGeometryNode.cpp"
    "src/raytracing/geometry/forms/meshes/textureCoordinates.cpp"
    "src/raytracing/geometry/forms/meshes/vertices.cpp"
    "src/raytracing/geometry/forms/Plane.cpp"
    "src/raytracing/geometry/forms/Sphere.cpp"
    "src/raytracing/geometry/ObjectGeometry.cpp"
    "src/raytracing/geometry/partitioning/GeometryNode.cpp"
    "src/raytracing/geometry/partitioning/KDTreeBalancer.cpp"
    "src/raytracing/geometry/partitioning/KDTreeBounding.cpp"
    "src/raytracing/geometry/partitioning/KDTreeBuildParameters.cpp"
    "src/raytracing/geometry/partitioning/KDTreeNode.cpp"
    "src/raytracing/geometry/partitioning/KDTreeNodeChilds.cpp"
    "src/raytracing/geometry/partitioning/KDTreePlane.cpp"
    "src/raytracing/geometry/partitioning/KDTreeRoot.cpp"
    "src/raytracing/geometry/partitioning/treebalancer/ArithmeticMeanKDTreeBalancer.cpp"
    "src/raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.cpp"
    "src/raytracing/geometry/partitioning/treebalancer/FixedIterationsSAHKDTreeBalancer.cpp"
    "src/raytracing/geometry/partitioning/treebalancer/MaxAxisKDTreeBalancer.cpp"
    "src/raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.cpp"
    "src/raytracing/geometry/partitioning/treebalancer/RotatingAxisKDTreeBalancer.cpp"
    "src/raytracing/geometry/partitioning/treebalancer/SAHKDTreeBalancer.cpp"
    "src/raytracing/geometry/partitioning/treebalancer/SamplingKDTreeBalancer.cpp"
    "src/raytracing/geometry/SceneGeometry.cpp"
    "src/raytracing/geometry/SceneIntersection.cpp"
    "src/raytracing/geometry/TransformInfo.cpp"
    "src/raytracing/IlluminatedIntersection.cpp"
    "src/raytracing/RaytraceConfiguration.cpp"
    "src/raytracing/RaytraceObserver.cpp"
    "src/raytracing/RaytraceOutput.cpp"
    "src/raytracing/RaytraceParameters.cpp"
    "src/raytracing/Raytracer.cpp"
    "src/raytracing/RaytracerCache.cpp"
    "src/raytracing/RaytracerPackets.cpp"
    "src/raytracing/scene/Camera.cpp"
    "src/raytracing/scene/Resources.cpp"
    "src/raytracing/scene/Scene.cpp"
    "src/raytracing/scene/SceneObject.cpp"
    "src/raytracing/shading/LightInfo.cpp"
    "src/raytracing/shading/LightShading.cpp"
    "src/raytracing/shading/ObjectShader.cpp"
    "src/raytracing/shading/PerLightShadowCache.cpp"
    "src/raytracing/shading/Raytrace.cpp"
    "src/raytracing/shading/RaytraceBaseParameters.cpp"
    "src/raytracing/shading/SceneShader.cpp"
    "src/raytracing/shading/SceneShaderContainment.cpp"
    "src/raytracing/shading/shaders/EnvironmentShader.cpp"
    "src/raytracing/shading/shaders/HDRImageShader.cpp"
    "src/raytracing/shading/shaders/IntersectionNormalShader.cpp"
    "src/raytracing/shading/SurfaceShading.cpp"
    "src/raytracing/shading/TextureTransformInfo.cpp"
)
source_group("Source files" FILES ${SOURCE_FILES})

set(ALL_FILES
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE raytracing)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
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
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vectorization/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vectorization/include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
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
        "_LIB;"
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
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /permissive-;
                /Od;
                /sdl;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy;
                /GS-;
                /openmp
            >
            /std:c++17;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /wd4514;
            /wd4710;
            /wd4711;
            /wd4820;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /permissive-;
                /Od;
                /sdl;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy;
                /GS-;
                /openmp
            >
            /std:c++17;
            /arch:SSE2;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /wd4514;
            /wd4710;
            /wd4711;
            /wd4820;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    vectorization
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    vectorization
)

