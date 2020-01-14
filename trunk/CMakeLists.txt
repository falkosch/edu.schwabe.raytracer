cmake_minimum_required(VERSION 3.9)

project("Raytracer")

set_property(
    GLOBAL
    PROPERTY USE_FOLDERS ON
)

add_subdirectory(vectorization)
add_subdirectory(vectorization.native-test)
add_subdirectory(raytracing)
add_subdirectory(raytracerui)
