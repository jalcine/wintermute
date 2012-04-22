##
## Bunch of useful macros and functions
## @author Adrian Borcuki <adrian@thesii.org>
## @author Jacky Alcine <jacky.alcine@thesii.org>
##

find_package(PkgConfig)
include(WintermuteDefaults)

## Set up an installation target.
#if (NOT TARGET uninstall)
#    set(CMAKE_UNINSTALL_TEMPLATE "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in")
#    if (PROJECT_LABEL STREQUAL "Wintermute")
#        set(CMAKE_UNINSTALL_TEMPLATE "${WINTER_"
#    configure_file("${CMAKE_UNINSTALL_TEMPLATE}"
#                    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
#    add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
#endif(NOT TARGET uninstall)

## Set up documentation.
macro(wntr_install_docs _path)
    if (NOT _wntr_set_docs)
        find_package(Doxygen)
        if(DOXYGEN_FOUND)
            find_program(HAVE_DOT dot)
            if(HAVE_DOT)
                message(STATUS "Found 'dot' program, Doxygen will use it to generate graphs for documentation.")
                set(HAVE_DOT YES)
            else(HAVE_DOT)
                set(HAVE_DOT NO)
            endif(HAVE_DOT)

            set(_wntr_set_docs true)
            set(_prefix)
            set(_docfile_prefix)

            set(_prefix "${CMAKE_SOURCE_DIR}/cmake")
            set(_docfile_prefix "${CMAKE_SOURCE_DIR}/data/res")
            set(_docfile_logo "${_docfile_prefix}/wintermute.png")

            configure_file("${CMAKE_SOURCE_DIR}/doc/Doxyfile.in"
                        "${PROJECT_BINARY_DIR}/Doxyfile" @ONLY)

            add_custom_target(docs
                ${DOXYGEN_EXECUTABLE} Doxyfile
                WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                COMMENT "Generating API documentation with Doxygen...")

            install(DIRECTORY ${PROJECT_BINARY_DIR}/doc/html/
                    DESTINATION ${_path})

        endif(DOXYGEN_FOUND)
    endif (NOT _wntr_set_docs)
endmacro(wntr_install_docs _path)

MACRO(PKGCONFIG_GETVAR _package _var _output_variable)
  SET(${_output_variable})

  # if pkg-config has been found
  IF(PKG_CONFIG_FOUND)

    EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS ${_package} --exists RETURN_VALUE _return_VALUE OUTPUT_VARIABLE _pkgconfigDevNull )

    # and if the package of interest also exists for pkg-config, then get the information
    IF(NOT _return_VALUE)

      EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS ${_package} --variable ${_var} OUTPUT_VARIABLE ${_output_variable} )

    ELSE(NOT _return_VALUE)
      MESSAGE(WARNING "${_package} not found.")
    ENDIF(NOT _return_VALUE)

  ELSE(PKG_CONFIG_FOUND)
    MESSAGE(ERROR "PkgConfig not found.")
  ENDIF(PKG_CONFIG_FOUND)

ENDMACRO(PKGCONFIG_GETVAR _package _var _output_variable)

macro(dbus_add_activation_service _sources)
    PKGCONFIG_GETVAR(dbus-1 session_bus_services_dir _install_dir)
    foreach (_i ${_sources})
        get_filename_component(_service_file ${_i} ABSOLUTE)
        string(REGEX REPLACE "\\.service.*$" ".service" _output_file ${_i})
        set(_target ${CMAKE_CURRENT_BINARY_DIR}/${_output_file})
        configure_file(${_service_file} ${_target})
        install(FILES ${_target} DESTINATION ${_install_dir})
    endforeach (_i ${ARGN})
endmacro(dbus_add_activation_service _sources)
