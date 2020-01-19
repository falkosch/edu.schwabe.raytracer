find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
    message(STATUS "Disabled generation of doxygen documentation (missing doxygen).")
    return()
endif()

set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(doxyfile_src "doxyfile.in")
set(doxyfile_out "${CMAKE_CURRENT_BINARY_DIR}/doxyfile")
set(doxyfile_html "${CMAKE_CURRENT_BINARY_DIR}/html")

get_filename_component(doxyfile_path ${doxyfile_src} PATH)
if(NOT path)
    set(doxyfile_path ${CMAKE_CURRENT_SOURCE_DIR})
endif()

configure_file(${doxyfile_src} ${doxyfile_out})

add_custom_target("api-docs"
    DEPENDS
        sources
        ${doxyfile_out}
    BYPRODUCTS
        "${doxyfile_html}/index.html"
    WORKING_DIRECTORY
        ${doxyfile_path}
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${doxyfile_path} ${doxyfile_html}
    COMMAND
        ${DOXYGEN} \"${doxyfile_out}\"
)

install(
    DIRECTORY ${doxyfile_html}
    DESTINATION "docs"
    COMPONENT docs
)
