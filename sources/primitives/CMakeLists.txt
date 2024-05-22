project(primitives
        LANGUAGES CXX
        VERSION 1.0.0
)

set(SOURCES
        src/primitives/AxisAlignedBoundingBox.cpp
        src/primitives/BoundingSphere.cpp
        src/primitives/Facet.cpp
        src/primitives/FacetEdges.cpp
        src/primitives/Ray.cpp
        src/primitives/RayCast.cpp
        src/primitives/SplittingPlane.cpp
)

set(HEADERS
        include/primitives.h
        include/primitives/AxisAlignedBoundingBox.h
        include/primitives/BoundingSphere.h
        include/primitives/Facet.h
        include/primitives/FacetEdges.h
        include/primitives/Ray.h
        include/primitives/RayCast.h
        include/primitives/SplittingPlane.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        PREFIX "Files"
        REGULAR_EXPRESSION ".+\\.(h|cpp)"
)

add_dependencies(${PROJECT_NAME} vectorization)

target_link_libraries(${PROJECT_NAME}
        PUBLIC vectorization
)

target_include_directories(${PROJECT_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE _LIB
)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
