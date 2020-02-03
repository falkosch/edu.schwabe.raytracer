enable_testing()

add_subdirectory("vectorization.native-test")

add_custom_target(tests)
add_custom_target(reports)
