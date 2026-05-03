project(benchmarks LANGUAGES CXX VERSION 1.0.0)

set(SOURCES
        src/intersection_benchmarks.cpp
        src/main.cpp
)

set(HEADERS
        include/intersection_benchmarks.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" REGULAR_EXPRESSION ".+\\.(h|cpp)")

add_dependencies(${PROJECT_NAME} raytracing)

target_link_libraries(${PROJECT_NAME} PRIVATE raytracing logging)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:CONSOLE)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:CONSOLE)
    endif ()

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
