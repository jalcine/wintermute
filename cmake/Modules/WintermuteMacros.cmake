# - Useful macros and functionality for Wintermute development.
# This module provides a collection of macros and does some tidy work foreach(
# Wintermute development.
#

#=============================================================================
# Copyright (c) 2012 Jacky Alcine   <jacky.alcine@thesii.org>
#           (c) 2012 Adrian Borucki <adrian@thesii.org>
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_package(PkgConfig)

include(WintermuteDefaults)
include(UseWintermute)

## Set up documentation.
macro(wntr_make_docs)
    set(__var_optional USE_EXAMPLES)
    set(__var_single   EXAMPLE_PATH
                       EXCLUDE_PATH
                       IMAGE_PATH
                       VERSION
                       PATH)

    cmake_parse_arguments(docs "${__var_optional}" "${__var_single}" "" ${ARGN})

    message("Documentation Sources Path: ${docs_PATH}")
    message("${ARGN}")
    include(Documentation)
    if (DOXYGEN_FOUND)
        configure_file("${WINTER_CURRENT_CMAKE_DIR}/Doxyfile.in"
                        "${PROJECT_BINARY_DIR}/Doxyfile" @ONLY)

        add_custom_target(docs
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen...")

        install(DIRECTORY ${PROJECT_BINARY_DIR}/doc/html/
                DESTINATION ${docs_PATH})

    else (DOXYGEN_FOUND)
        message(STATUS "Doxygen wasn't found; can't generate documentation of C/C++ sources.")
    endif(DOXYGEN_FOUND)
endmacro(wntr_make_docs)

macro(PKGCONFIG_GETVAR _package _var _output_variable)
  set(${_output_variable})

  # if pkg-config has been found
  if(PKG_CONFIG_FOUND)

    execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} ${_package} --exists
                    RESULT_VARIABLE _return_VALUE
                    OUTPUT_VARIABLE _pkgconfigDevNull)

    # and if the package of interest also exists for pkg-config, then get the information
    if(NOT _return_VALUE)
      execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} ${_package} --variable ${_var}
                      OUTPUT_VARIABLE ${_output_variable})

    else(NOT _return_VALUE)
      message(WARNING "PkgConfig: ${_package} not found.")
    endif(NOT _return_VALUE)

  else(PKG_CONFIG_FOUND)
    message(ERROR "PkgConfig not found.")
  endif(PKG_CONFIG_FOUND)

endmacro(PKGCONFIG_GETVAR _package _var _output_variable)

macro(dbus_add_activation_service _sources)
    pkgconfig_getvar(dbus-1 session_bus_services_dir _install_dir)
    foreach (_i ${_sources})
        get_filename_component(_service_file ${_i} ABSOLUTE)
        string(REGEX REPLACE "\\.service.*$" ".service" _output_file ${_i})
        set(_target ${CMAKE_CURRENT_BINARY_DIR}/${_output_file})
        configure_file(${_service_file} ${_target})
        install(FILES ${_target} DESTINATION ${_install_dir})
    endforeach (_i ${ARGN})
endmacro(dbus_add_activation_service _sources)

macro(winter_define_plugin)
    set(__val_optional ENABLED)
    set(__val_multi DEPENDS_PLUGINS
                    DEPENDS_PACKAGES)
    set(__val_single BLURB
                     AUTHOR_NAME
                     AUTHOR_EMAIL
                     VENDOR_NAME
                     VENDOR_EMAIL
                     VERSION_COMPAT
                     VERSION_PLUGIN
                     NAME_FRIENDLY
                     NAME_TARGET
                     UUID
                     URL
                     TYPE
                     )
    cmake_parse_arguments(WDP "${__val_optional}"
                              "${__val_single}"
                              "${__val_multi}"
                         ${ARGN})

    if (NOT DEFINED WDP_ENABLED)
        set(WPD_ENABLED "true")
    endif (NOT DEFINED WDP_ENABLED)

    set(__specpath "${PROJECT_BINARY_DIR}/${WDP_UUID}.spec")
    configure_file("${WINTER_CURRENT_CMAKE_DIR}/pluginspec.in"
                   "${__specpath}" @ONLY)

    install(FILES "${__specpath}"
            DESTINATION "${WINTER_PLUGIN_SPEC_INSTALL_DIR}")
endmacro(winter_define_plugin)

macro(winter_install_plugin)
    set(__val_optional)
    set(__val_single TARGET)
    set(__val_multi)

    cmake_parse_arguments(WIP "${__val_optional}"
                              "${__val_single}"
                              "${__val_multi}"
                          ${ARGN})

    install(TARGETS ${WIP_TARGET}
        EXPORT "${WIP_TARGET}LibraryDepends"
        LIBRARY DESTINATION "${WINTER_PLUGIN_INSTALL_DIR}")

    install(EXPORT "${WIP_TARGET}LibraryDepends"
        DESTINATION "${WNTRDATA_CMAKE_DIR}")

    install(TARGETS ${WIP_TARGET}
            LIBRARY DESTINATION "${WINTER_PLUGIN_INSTALL_DIR}")

    # Let the build system know you're building a plug-in.
    set(WINTER_IS_PLUGIN ON)
endmacro(winter_install_plugin)

macro(winter_define_api)
    set(__val_optional)
    set(__val_single UUID
                     NAME_FRIENDLY)
    set(__val_multi)

    cmake_parse_arguments(WAPI
        "${__val_optional}" "${__val_single}" "${__val_multi}"
        ${ARGN})

    set(_WAPI_FEATURE_NAME "_feature_${WAPI_UUID}")

endmacro(winter_define_api)

macro(winter_define_backend)
set(__val_optional)
    set(__val_single UUID
                     NAME_FRIENDLY)
    set(__val_multi)

    cmake_parse_arguments(WBACKEND
        "${__val_optional}" "${__val_single}" "${__val_multi}"
        ${ARGN})
endmacro(winter_define_backend)