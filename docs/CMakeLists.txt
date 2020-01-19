add_custom_target(docs
    DEPENDS
        sources
)

add_subdirectory(api-docs)
add_subdirectory(manual)
