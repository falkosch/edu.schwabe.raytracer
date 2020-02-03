add_subdirectory(api-docs)
add_subdirectory(manual)

add_custom_target(docs
    DEPENDS
        api-docs
        manual
)
