find_package(LATEX)
if (NOT LATEX_FOUND)
    message(STATUS "Disabled generation of documentation (missing LaTeX).")
    return()
endif ()

set(manual_src "${CMAKE_CURRENT_SOURCE_DIR}/manual.tex")
set(manual_out "${CMAKE_CURRENT_BINARY_DIR}/manual.pdf")

add_custom_target("docs-manual"
        DEPENDS ${manual_src}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMAND ${PDFLATEX_COMPILER} ${manual_src}
        COMMAND ${PDFLATEX_COMPILER} ${manual_src}
        COMMAND ${PDFLATEX_COMPILER} ${manual_src}
        BYPRODUCTS ${manual_out} "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
)

install(
        FILES ${manual_out}
        DESTINATION docs
        COMPONENT docs
)
