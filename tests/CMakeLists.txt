add_custom_target(tests
    DEPENDS
        sources
)

enable_testing()

add_subdirectory("vectorization.native-test")
