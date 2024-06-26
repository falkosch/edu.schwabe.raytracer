cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project("sources-build-aggregate" LANGUAGES CXX)

# Generates a dummy compilation source so that add_library doesn't complain
set(SOURCES_BUILD_AGGREGATE_DUMMY_FILE "${CMAKE_CURRENT_BINARY_DIR}/dummy.cxx")
file(GENERATE OUTPUT ${SOURCES_BUILD_AGGREGATE_DUMMY_FILE} CONTENT "")

add_library(${PROJECT_NAME} STATIC ${SOURCES_BUILD_AGGREGATE_DUMMY_FILE})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        $<$<STREQUAL:${CMAKE_VS_PLATFORM_NAME},x64>: X64 >
        $<$<STREQUAL:${CMAKE_VS_PLATFORM_NAME},x86>: X86 >
        $<$<CONFIG:Release>: NDEBUG >
        $<$<CONFIG:Debug>: _DEBUG DEBUG >
)

set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME} PUBLIC stdc++)

    target_compile_options(${PROJECT_NAME}
            PUBLIC
            $<$<CONFIG:Release>:
            $<$<BOOL:${COVERAGE}>: -O0 >
            $<$<NOT:$<BOOL:${COVERAGE}>>: -O3 >
            >
            $<$<CONFIG:Debug>:
            $<$<BOOL:${COVERAGE}>: -O0 >
            $<$<NOT:$<BOOL:${COVERAGE}>>: -Og >
            >
    )

    target_link_options(${PROJECT_NAME}
            PUBLIC
            $<$<CONFIG:Release>:
            $<$<BOOL:${COVERAGE}>: -O0 >
            $<$<NOT:$<BOOL:${COVERAGE}>>: -O3 >
            >
            $<$<CONFIG:Debug>:
            $<$<BOOL:${COVERAGE}>: -O0 >
            $<$<NOT:$<BOOL:${COVERAGE}>>: -Og >
            >
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME}
            PUBLIC
            $<$<CONFIG:Release>:
            $<$<NOT:$<BOOL:${COVERAGE}>>: /Oy >
            >
    )

    target_link_options(${PROJECT_NAME}
            PUBLIC
            $<$<CONFIG:Release>: /OPT:REF /OPT:ICF >
            $<$<CONFIG:Debug>: /Debug >
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
