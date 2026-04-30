enable_testing()

add_subdirectory("vectorization.native-test")
add_subdirectory("primitives.native-test")
add_subdirectory("raytracing.native-test")

add_custom_target(tests)
