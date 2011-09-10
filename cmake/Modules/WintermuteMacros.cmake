## Bunch of useful macros and functions
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
        configure_file("${PROJECT_SOURCE_DIR}/Doxyfile.in" Doxyfile)
        add_custom_target(doxydoc
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen")
    endif()
endmacro()
