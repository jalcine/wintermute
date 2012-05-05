# - Useful macros and functionality for Wintermute development.
# This module provides a collection of macros and does some tidy work foreach(
# Wintermute development.
#
#
# macro wntr_make_docs(PATH <path>
#                      VERSION <version>)
#
# macro wntr

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
                       INSTALL_PATH
                       VERSION
                       PATH)

    cmake_parse_arguments(docs "${__var_optional}" "${__var_single}" "" ${ARGN})

    include(Documentation)
    if (BUILD_DOCUMENTATION)
        if (DOXYGEN_FOUND)
            configure_file("${WINTER_CURRENT_CMAKE_DIR}/Doxyfile.in"
                            "${PROJECT_BINARY_DIR}/Doxyfile" @ONLY)

            add_custom_target(docs
                ${DOXYGEN_EXECUTABLE} Doxyfile
                WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                COMMENT "Generating API documentation with Doxygen...")

            install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html/
                    DESTINATION ${docs_INSTALL_PATH})

        else (DOXYGEN_FOUND)
            message(STATUS "Doxygen wasn't found; can't generate documentation of C/C++ sources.")
        endif(DOXYGEN_FOUND)
    endif(BUILD_DOCUMENTATION)
endmacro(wntr_make_docs)

macro(pkgconfig_getvar _package _var _output_variable)
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
    message(FATAL_ERROR "PkgConfig not found.")
  endif(PKG_CONFIG_FOUND)

endmacro(pkgconfig_getvar _package _var _output_variable)


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

macro(wntr_define_plugin)
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
endmacro(wntr_define_plugin)

macro(wntr_install_plugin)
    set(__val_optional GENERATE_DOCS)
    set(__val_single   TARGET
                       PACKAGE_PREFIX
                       HEADERS_PATH
                       HEADERS_DESTINATION
                       EXPORT_NAME)
    set(__val_multi    )

    cmake_parse_arguments(WIP "${__val_optional}"
                              "${__val_single}"
                              "${__val_multi}"
                          ${ARGN})

    set(__TARGET_EXPORT "${WIP_EXPORT_NAME}LibraryDepends")
    set(__FIND_PLUGIN  "${CMAKE_BINARY_DIR}/Find${WIP_EXPORT_NAME}.cmake")
    set(__DBUS_SERVICE "${CMAKE_BINARY_DIR}/org.thesii.Wintermute.Plugin.${WIP_DBUS_NAME}.service")

    set_target_properties(${WIP_TARGET} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        OUTPUT_NAME              "${WIP_TARGET}"
        PREFIX                   "lib"
        SUFFIX                   ".so")

    get_target_property(tp ${WIP_TARGET} PREFIX)
    get_target_property(ts ${WIP_TARGET} SUFFIX)
    get_target_property(to ${WIP_TARGET} OUTPUT_NAME)
    set(${WIP_PACKAGE_PREFIX}_LIBRARY      "${WINTER_PLUGIN_INSTALL_DIR}/${tp}${to}${ts}")
    set(${WIP_PACKAGE_PREFIX}_LIBRARY_DIR "${WINTER_PLUGIN_INSTALL_DIR}")

    configure_file("${WINTER_CURRENT_CMAKE_DIR}/FindPlugin.cmake.in"
                   "${__FIND_PLUGIN}" @ONLY)
    configure_file("${__FIND_PLUGIN}" "${__FIND_PLUGIN}" @ONLY)

    configure_file("${WINTER_CURRENT_CMAKE_DIR}/dbus_service.cmake.in"
                   "${__DBUS_SERVICE}" @ONLY)

    dbus_add_activation_service(${__DBUS_SERVICE})

    install(TARGETS ${WIP_TARGET}
            EXPORT "${__TARGET_EXPORT}"
            LIBRARY DESTINATION "${WINTER_PLUGIN_INSTALL_DIR}")

    install(EXPORT "${__TARGET_EXPORT}"
            DESTINATION "${WINTER_CMAKE_DIR}")

    install(FILES "${__FIND_PLUGIN}"
            DESTINATION "${WINTER_CMAKE_DIR}")

    wntr_install_headers(HEADERS_PATH  "${WIP_HEADERS_PATH}"
                         DESTINATION   "${WIP_HEADERS_DESTINATION}")
# Let the build system know you're building a plug-in.
    set(WINTER_IS_PLUGIN ON)
endmacro(wntr_install_plugin)

macro(wntr_install_headers)
    set(__val_optional)
    set(__val_single   HEADERS_PATH
                       DESTINATION)
    set(__val_multi)

    cmake_parse_arguments(WIH "${__val_optional}"
                              "${__val_single}"
                              "${__val_multi}"
                          ${ARGN})

    file(GLOB_RECURSE headers RELATIVE "${WIH_HEADERS_PATH}" "*.hpp")
    foreach(header ${headers})
        set(__file )
        set(__pos -1)
        set(__prefix )
        set(__path "${WIH_DESTINATION}")
        string(REPLACE "//" "/" header "${header}")
        string(REPLACE "${PROJECT_SOURCE_DIR}/" "" header "${header}")
        string(REPLACE "${__prefix}" "" __file "${__prefix}")
        string(FIND "${header}" "/" __pos REVERSE)
        if (NOT(__pos EQUAL -1))
            string(SUBSTRING "${header}" ${__pos} -1 __file)
            string(REPLACE "/" "" __file "${__file}")
            string(REPLACE "/${__file}" "" __prefix "${header}")
            set(__prefix "/${__prefix}")
            set(__path "${__path}${__prefix}")
        else (NOT(__pos EQUAL -1))
            set(__prefix )
        endif (NOT(__pos EQUAL -1))

        install(FILES ${header}
                DESTINATION "${__path}")
    endforeach(header)
endmacro(wntr_install_headers)

macro(wntr_automoc _sources)
    foreach(_source ${_sources})
        get_filename_component(_filename "${_source}" NAME)
        string(REPLACE "${_filename}" "" _path "${_source}")
        string(REPLACE "${PROJECT_SOURCE_DIR}/" "" _path "${_path}")
        file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${_path}")
    endforeach(_source  ${_sources})
    qt4_automoc(${_sources})
endmacro(wntr_automoc _sources)