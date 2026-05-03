project(logging LANGUAGES CXX VERSION 1.0.0)

set(SOURCES
        src/logging/DisplayBuffer.cpp
        src/logging/LogScope.cpp
        src/logging/Logger.cpp
)

set(HEADERS
        include/logging.h
        include/logging/DisplayBuffer.h
        include/logging/LogEntry.h
        include/logging/LogLevel.h
        include/logging/LogScope.h
        include/logging/Logger.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" REGULAR_EXPRESSION ".+\\.(h|cpp)")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_definitions(${PROJECT_NAME} PRIVATE _LIB)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
