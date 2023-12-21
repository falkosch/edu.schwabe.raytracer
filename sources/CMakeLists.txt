add_subdirectory(sources-build-aggregate)
add_subdirectory(vectorization)
add_subdirectory(raytracing)
add_subdirectory(raytracerui)

add_custom_target(sources DEPENDS raytracerui)
