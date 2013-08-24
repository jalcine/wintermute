###############################################################################
### Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

include(CMakeParseArguments)
include(WintermuteVariables)

## Determine the location of the necessary configuration file for Wintermute's 
## plugin.

if (CMAKE_PROJECT_NAME EQUAL "Wintermute")
  set(WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE "${CMAKE_SOURCE_DIR}/cmake/Templates/PluginDefinition.spec.in")
else(CMAKE_PROJECT_NAME EQUAL "Wintermute")
  set(WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE "${WINTERMUTE_CMAKE_TEMPLATES_DIR}/PluginDefinition.spec.in")
endif(CMAKE_PROJECT_NAME EQUAL "Wintermute")

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
  set(_oneArgs   TARGET UUID)
  set(_multiArgs )
  cmake_parse_arguments(wdp "" "${_oneArgs}" "${_multiArgs}" ${ARGN})

  # Define the plugin's CMake prefix.
  string(TOUPPER "WINTERMUTE_PLUGIN_${wdp_TARGET}" _local)
  string(TOLOWER ${wdp_TARGET} _minLocal)
  string(TOUPPER ${wdp_TARGET}   ${_local}_EXPORT_SYMBOL)
  
  # Define the plugin's CMake properties.
  set("${_local}_SOURCES"         ${wdp_SOURCES}                 CACHE STRING "Sources.")
  set("${_local}_TARGET"          "wintermute-${wdp_TARGET}"     CACHE STRING "Target.")
  set("${_local}_UUID"            ${wdp_UUID}                    CACHE STRING "Uuid.")
  set("${_local}_LIBRARIES"       ${WINTERMUTE_LIBRARIES}        CACHE STRING "Libraries.")
  set("${_local}_INCLUDE_DIRS"    ${WINTERMUTE_INCLUDE_DIRS}
                                  ${WINTERMUTE_INCLUDE_DIR}      CACHE STRING "Directories.")
  set("${_local}_HEADERS_PATH"    "${WINTERMUTE_INCLUDE_DIR}/plugins/${_minLocal}" CACHE STRING "Headers install.")
  set("${_local}_DEFINITION_FILE" "${CMAKE_BINARY_DIR}/plugin-${wdp_UUID}.spec" CACHE STRING "Def.")

endfunction(wintermute_plugin_declare)

##
## @fn wintermute_plugin_target_declare
## @brief Handles the build-time requirements for a Wintermute plug-in.
##
function(wintermute_plugin_target_declare)
  set(_oneArgs   TARGET)
  set(_multiArgs SOURCES)
  cmake_parse_arguments(wptd "" "${_oneArgs}" "${_multiArgs}" ${ARGN})

  # Define the plugin's CMake prefix.
  string(TOUPPER "WINTERMUTE_PLUGIN_${wptd_TARGET}" _local)
  string(TOLOWER ${wptd_TARGET} _minLocal)
  set("${_local}_VERSION" "${${_local}_PLUGIN_VERSION_MAJOR}.${${_local}_PLUGIN_VERSION_MINOR}.${${_local}_PLUGIN_VERSION_PATCH}")

  # Ensure that we handle the automagically moc-ing of files.
  qt4_automoc(${wptd_SOURCES})
  
  # Define the library.
  add_library("${${_local}_TARGET}" SHARED ${wptd_SOURCES})

  # Coat the target with Wintermute's build options.
  wintermute_add_properties(${${_local}_TARGET})
  
  # Define the library's version.
  set_target_properties(${${_local}_TARGET} PROPERTIES
    FOLDER        "Wintermute/${${_local}_TARGET}"
    EXPORT_SYMBOL "${${_local}_EXPORT_SYMBOL}"
    VERSION       ${${_local}_VERSION}
    SOVERSION     ${${_local}_VERSION}
    INCLUDE_DIRECTORIES "${${_local}_INCLUDE_DIRS}"
  )

  # Set up linking.
  target_link_libraries(${${_local}_TARGET} ${WINTERMUTE_LIBRARIES} ${${_local}_LIBRARIES})

  message(STATUS "Plugin '${${_local}_TARGET}' version ${${_local}_VERSION} defined.")
endfunction(wintermute_plugin_target_declare)

## TODO: Document this method.
## TODO: Implement this method.
function(wintermute_plugin_generate_documentation)
  message(WARNING "[cmake] Documentation function not yet built.")
endfunction(wintermute_plugin_generate_documentation)

##
## @fn wintermute_plugin_add_include_directories
## @brief Adds include directories to the plug-in.
## @param TARGET       The plug-in's target name.
## @param DIRECTORIES  The directories to be added for inclusion in the build.
function(wintermute_plugin_add_include_directories)
  cmake_parse_arguments(wpad "" "TARGET" "DIRECTORIES" ${ARGN})

  string(TOUPPER "WINTERMUTE_PLUGIN_${wpad_TARGET}" _local)
  list(APPEND ${_local}_INCLUDE_DIRECTORIES ${wpad_DIRECTORIES} ${WINTERMUTE_INLCUDE_DIRS})
  list(REMOVE_DUPLICATES ${_local}_INCLUDE_DIRECTORIES)
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

  list(APPEND ${_local}_LIBRARIES ${wpal_LIBRARIES} ${WINTERMUTE_LIBRARIES})
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

  string(TOUPPER "WINTERMUTE_PLUGIN_${wpc_TARGET}_" _local)
  
  foreach(_validProperty ${_validProperties})
    set("${_local}${_validProperty}" "${wpc_${_validProperty}}" CACHE STRING "Configuration property.")
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
  string(TOUPPER "WINTERMUTE_PLUGIN_${wpsv_TARGET}_" _local)

  foreach(_pluginVersionVariable ${_pluginVersions})
    set("${_local}${_pluginVersionVariable}" "${wpsv_${_pluginVersionVariable}}" CACHE STRING "Versioning.")
  endforeach(_pluginVersionVariable ${_pluginVersions})

  foreach(_systemVersionVariable ${_systemVersions})
    set("${_local}${_systemVersionVariable}" "${wpsv_${_systemVersionVariable}}" CACHE STRING "System versioning.")
  endforeach(_systemVersionVariable ${_systemVersions})

endfunction(wintermute_plugin_set_version)

##
## @fn wintermute_plugin_install
## @brief Enacts the necessary work to handle the installation process of 
##        a plug-in for Wintermute.
##
function(wintermute_plugin_install)
  cmake_parse_arguments(wpi "" "TARGET" "" ${ARGN})
  string(TOUPPER "WINTERMUTE_PLUGIN_${wpi_TARGET}_" _local)

  # DONE: Define the plug-in's definition file.
  set(${_local}DEFINITION_FILE "${CMAKE_BINARY_DIR}/${${_local}UUID}.spec")
  configure_file(${WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE} ${${_local}DEFINITION_FILE})
 
  # DONE: Install the library itself.
  install(TARGETS        ${${_local}TARGET}
    LIBRARY DESTINATION  ${WINTERMUTE_PLUGIN_LIBRARY_DIR}  
  )

  # TODO: Install exported information.
  # TODO: Install build-time headers.
  # TODO: Install documentation.

  # DONE: Generate the definition file.
  # DONE: Install the definition file.
  install(FILES ${${_local}DEFINITION_FILE}
    DESTINATION ${WINTERMUTE_PLUGIN_DEFINITION_DIR})
endfunction(wintermute_plugin_install)
