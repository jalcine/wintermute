## Bunch of useful macros and functions

# Fixes the local paths to represent full, absolute paths.
macro(winter_make_absolute paths)
    foreach(in paths)
        set(p ${PROJECT_CAPITAL_NAME}_${in}_DIR)
        if(NOT IS_ABSOLUTE "${${p}}")
            set(${p} "${CMAKE_INSTALL_PREFIX}/${${p}}")
        endif()
    endforeach()
endmacro(winter_make_absolute)

macro(enable_doxygen)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        configure_file("${PROJECT_SOURCE_DIR}/Doxyfile.in" "${PROJECT_BINARY_DIR}/Doxyfile")
        add_custom_target(doxydoc
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen...")
    endif()
endmacro()
