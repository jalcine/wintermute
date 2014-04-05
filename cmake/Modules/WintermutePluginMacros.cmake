###############################################################################
### Copyright (C) 2013 - 2014 Jacky Alciné <me@jalcine.me>
###
### This file is part of Wintermute, the extensible AI platform.
###
### Wintermute is free software; you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation; either version 3 of the License, or
### (at your option) any later version.
###
### Wintermute is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

if (EXISTS wntrpluginmacros)
  return()
else()
  set(wntrpluginmacros ON CACHE BOOLEAN TRUE)
endif()

include(CMakeParseArguments)
include(WintermuteVariables)

## Determine the location of the necessary configuration file for Wintermute's 
## plugin.

if (NOT DEFINED WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE)
  if (NOT TARGET wintermute)
    set(WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE
      "${WINTERMUTE_CMAKE_TEMPLATES_DIR}/PluginDefinition.spec.in")
  else()
    set(WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE
      "${CMAKE_SOURCE_DIR}/cmake/Templates/PluginDefinition.spec.in")
  endif()
endif()

##
## @fn wintermute_plugin_declare
## @brief Defines a plug-in for building and use in Wintermute.
## @param TARGET   The name of the plug-in's target.
## @param SOURCES  The sources to be used for building the plug-in.
##
## @note You should use `wintermute_plugin_configure` at least once after using 
##       this.
##
function(wintermute_plugin_declare)
  set(_oneArgs   TARGET)
  set(_multiArgs SOURCES HEADERS)
  cmake_parse_arguments(wdp "" "${_oneArgs}" "${_multiArgs}" ${ARGN})

  # Define the plugin's CMake prefix.
  string(TOUPPER "WINTERMUTE_PLUGIN_${wdp_TARGET}" _local)
  string(TOLOWER ${wdp_TARGET} _minLocal)
  string(TOUPPER ${wdp_TARGET}_EXPORTS _sym)

  # Define the plugin's CMake properties.
  set(${_local}_SOURCES         ${wdp_SOURCES}
    CACHE STRING "Sources.")
  set(${_local}_HEADERS         ${wdp_HEADERS}
    CACHE STRING "Headers.")
  set(${_local}_TARGET          "wintermute-${wdp_TARGET}"
    CACHE STRING "Target.")
  set(${_local}_EXPORT_SYMBOL   ${_sym}
    CACHE STRING "Export symbol.")
  set(${_local}_LIBRARIES       ${WINTERMUTE_LIBRARIES}
    CACHE STRING "Libraries.")
  set(${_local}_INCLUDE_DIRS    ${WINTERMUTE_INCLUDE_DIRS}
                                ${WINTERMUTE_INCLUDE_DIR}
    CACHE STRING INTERNAL FORCE)
  set(${_local}_HEADERS_PATH    "${WINTERMUTE_PLUGIN_INCLUDE_DIR}/${_minLocal}"
    CACHE STRING "Headers install.")
  set(${_local}_DEFINITION_FILE "${CMAKE_BINARY_DIR}/plugin-${wdp_TARGET}.spec"
    CACHE STRING "Definitions file.")

endfunction(wintermute_plugin_declare)

##
## @fn wintermute_plugin_target_declare
## @brief Handles the build-time requirements for a Wintermute plug-in.
##
function(wintermute_plugin_target_declare)
  set(_oneArgs   TARGET)
  cmake_parse_arguments(wdp "" "${_oneArgs}" "" ${ARGN})

  # Define the plugin's CMake prefix.
  string(TOUPPER "WINTERMUTE_PLUGIN_${wdp_TARGET}" _local)
  string(TOLOWER "${wdp_TARGET}" _minLocal)

  set("${_local}_VERSION" "${${_local}_PLUGIN_VERSION_MAJOR}.${${_local}_PLUGIN_VERSION_MINOR}.${${_local}_PLUGIN_VERSION_PATCH}")

  # Define the library.
  add_library(${${_local}_TARGET} SHARED ${${_local}_SOURCES})

  # Coat the target with Wintermute's build options.
  wintermute_add_properties(${${_local}_TARGET})

  # Define the library's version.
  set_target_properties(${${_local}_TARGET} PROPERTIES
    FOLDER        "Wintermute/${${_local}_TARGET}"
    EXPORT_SYMBOL "${${_local}_EXPORT_SYMBOL}"
    VERSION       ${${_local}_VERSION}
    SOVERSION     ${${_local}_VERSION})

  target_link_libraries(${${_local}_TARGET} ${${_local}_LIBRARIES})

  include_directories(${${_local}_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

  if (TARGET wintermute)
    include_directories(${CMAKE_SOURCE_DIR}/src)
  else()
    include_directories(${WINTERMUTE_INCLUDE_DIR}
      ${WINTERMUTE_CORE_INCLUDE_DIR})
  endif(TARGET wintermute)

  message(STATUS "Plugin '${${_local}_TARGET}' v. ${${_local}_VERSION} defined.")
endfunction(wintermute_plugin_target_declare)

## TODO: Document this method.
## TODO: Implement this method.
function(wintermute_plugin_generate_documentation)
  set(_oneArgs   TARGET BRIEF NAME)
  cmake_parse_arguments(wpgd "" "${_oneArgs}" "" ${ARGN})

  # Define the plugin's CMake prefix.
  string(TOUPPER "WINTERMUTE_PLUGIN_${wpgd_TARGET}" _local)
  string(TOLOWER "${wpgd_TARGET}" _minLocal)
  wintermute_generate_documentation (TARGET ${${_local}_TARGET}
    NAME             ${${_local}_NAME}
    SOURCES          ${${_local}_SOURCES}
    BRIEF            ${${_local}_BRIEF}
    VERSION          ${${_local}_VERSION}
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/${${_local}_TARGET}
  )

endfunction(wintermute_plugin_generate_documentation)

##
## @fn wintermute_plugin_add_include_directories
## @brief Adds include directories to the plug-in.
## @param TARGET       The plug-in's target name.
## @param DIRECTORIES  The directories to be added for inclusion in the build.
function(wintermute_plugin_add_include_directories)
  cmake_parse_arguments(wpad "" "TARGET" "DIRECTORIES" ${ARGN})

  string(TOUPPER "WINTERMUTE_PLUGIN_${wpad_TARGET}" _local)
  list(APPEND ${_local}_INCLUDE_DIRECTORIES ${wpad_DIRECTORIES}
    ${WINTERMUTE_INCLUDE_DIRS})
endfunction(wintermute_plugin_add_include_directories)

##
## @fn wintermute_plugin_add_libraries
## @brief Adds libraries to the plug-in.
## @param TARGET     The plug-in's target name.
## @param LIBRARIES  A list of libraries to be appended to this library's 
##                   build configuration.
##
function(wintermute_plugin_add_libraries)
  cmake_parse_arguments(wpal "" "TARGET" "LIBRARIES" ${ARGN})
  string(TOUPPER "WINTERMUTE_PLUGIN_${wpal_TARGET}" _local)

  set(${_local}_LIBRARIES ${WINTERMUTE_LIBRARIES} ${wpal_LIBRARIES} CACHE
    STRING INTERNAL FORCE)
  list(REMOVE_DUPLICATES ${_local}_LIBRARIES)
endfunction(wintermute_plugin_add_libraries)

##
## @fn wintermute_plugin_configure
## @brief Defines meta-data information for this plugin.
##
## Meta-Data Collected:
## ------------------------------------------------------------------
## NAME_AUTHOR         | The name of the author(s) of the plug-in.
## NAME_RPC            | The name used over RPC.
## EMAIL               | E-mail for contact about this plug-in.
## URI                 | Web URI to location about this plug-in.
## ------------------------------------------------------------------
##
function(wintermute_plugin_configure)
  set(_validProperties AUTHOR_NAME
    NAME_RPC
    EMAIL
    URI
    )

  set(_options )
  set(_singleVals ${_validProperties} TARGET)
  cmake_parse_arguments(wpc "${_options}" "${_singleVals}" "" ${ARGN})

  string(TOUPPER "WINTERMUTE_PLUGIN_${wpc_TARGET}" _local)

  foreach(_validProperty ${_validProperties})
    set("${_local}_${_validProperty}" "${wpc_${_validProperty}}" 
      CACHE STRING "Configuration property.")
  endforeach(_validProperty ${_validProperties})

endfunction(wintermute_plugin_configure)

##
## @fn wintermute_plugin_set_version
## @brief Defines the version of the plugin.
##
function(wintermute_plugin_set_version)
  set(_pluginVersions PLUGIN_VERSION_MAJOR
    PLUGIN_VERSION_MINOR
    PLUGIN_VERSION_PATCH
    PLUGIN_VERSION_STAGE
    )

  set(_systemVersions SYSTEM_VERSION_MAJOR
    SYSTEM_VERSION_MINOR
    SYSTEM_VERSION_PATCH
    SYSTEM_VERSION_STAGE
    SYSTEM_VERSION_COMPARISON
    )

  set(_singleVals ${_pluginVersions} ${_systemVersions} TARGET)
  cmake_parse_arguments(wpsv "" "${_singleVals}" "" ${ARGN})
  string(TOUPPER "WINTERMUTE_PLUGIN_${wpsv_TARGET}" _local)

  foreach(_pluginVersionVariable ${_pluginVersions})
    set("${_local}_${_pluginVersionVariable}" 
      "${wpsv_${_pluginVersionVariable}}" 
      CACHE STRING "Versioning.")
  endforeach(_pluginVersionVariable ${_pluginVersions})

  foreach(_systemVersionVariable ${_systemVersions})
    set("${_local}_${_systemVersionVariable}" 
      "${wpsv_${_systemVersionVariable}}"
      CACHE STRING "System versioning.")
  endforeach(_systemVersionVariable ${_systemVersions})

endfunction(wintermute_plugin_set_version)

##
## @fn wintermute_plugin_install
## @brief Enacts the necessary work to handle the installation process of 
##        a plug-in for Wintermute.
##
function(wintermute_plugin_install)
  cmake_parse_arguments(wpi "" "TARGET" "" ${ARGN})
  string(TOUPPER "WINTERMUTE_PLUGIN_${wpi_TARGET}" _local)

  set(${_local}_DEFINITION_FILE "${CMAKE_BINARY_DIR}/${${_local}_TARGET}.spec")
  configure_file(${WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE} ${${_local}_DEFINITION_FILE})

  install(TARGETS        ${${_local}_TARGET}
    EXPORT               ${${_local}_TARGET}
    COMPONENT            Runtime
    LIBRARY DESTINATION  ${WINTERMUTE_PLUGIN_LIBRARY_DIR}
  )

  install(EXPORT         ${${_local}_TARGET}
    COMPONENT            Development
    CONFIGURATIONS       Debug
    DESTINATION          ${WINTERMUTE_CMAKE_MODULES_DIR}
  )

  install(FILES         ${${_local}_HEADERS}
    COMPONENT           Development
    CONFIGURATIONS      Debug
    DESTINATION         ${${_local}_HEADERS_PATH}
  )

  install(FILES ${${_local}_DEFINITION_FILE}
    COMPONENT           Runtime
    DESTINATION         ${WINTERMUTE_PLUGIN_DEFINITION_DIR})
endfunction(wintermute_plugin_install)
