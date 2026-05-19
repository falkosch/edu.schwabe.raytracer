project("primitives.native-test" LANGUAGES CXX VERSION 1.0.0)

set(SOURCES
        src/primitives/Ray_test.cpp
        src/primitives/RayCast_test.cpp
        src/primitives/AxisAlignedBoundingBox_test.cpp
        src/primitives/BoundingSphere_test.cpp
        src/primitives/Facet_test.cpp
        src/primitives/FacetEdges_test.cpp
        src/primitives/SplittingPlane_test.cpp
)

set(HEADERS
        include/primitives_tests.h
        src/stdafx.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" REGULAR_EXPRESSION ".+\\.(h|cpp)")

add_dependencies(${PROJECT_NAME} primitives)

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{VCInstallDir}/Auxiliary/VS/UnitTest/lib")

target_link_libraries(${PROJECT_NAME} PRIVATE primitives)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        "$ENV{VCInstallDir}/Auxiliary/VS/UnitTest/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/stdafx.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions)
    endif ()

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
