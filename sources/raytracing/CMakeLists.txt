project(raytracing
    LANGUAGES CXX
    VERSION 1.0.0
)

add_library(${PROJECT_NAME}
    STATIC
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

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Files"
    REGULAR_EXPRESSION ".+\\.(h|cpp)"
)

add_dependencies(${PROJECT_NAME}
    vectorization
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        vectorization
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        _LIB
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

else()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif()
