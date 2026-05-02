add_subdirectory(logging)
add_subdirectory(vectorization)
add_subdirectory(primitives)
add_subdirectory(raytracing)
add_subdirectory(raytracerui)
add_subdirectory(benchmarks)

add_custom_target(sources DEPENDS raytracerui benchmarks)
