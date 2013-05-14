###############################################################################
### Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
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
  set(_multiArgs SOURCES)
  cmake_parse_arguments(wdp "" "${_oneArgs}" "${_multiArgs}" ${ARGN})

  # Define the plugin's CMake prefix.
  string(TOUPPER "WINTERMUTE_PLUGIN_${wdp_TARGET}" _local)

  # Define the plugin's CMake properties.
  set("${_local}_SOURCES" "${wdp_SOURCES}"           CACHE STRING "Sources.")
  set("${_local}_TARGET"  "wintermute-${wdp_TARGET}" CACHE STRING "Target.")
  set("${_local}_DEFINITION_FILE" "${CMAKE_BUILD_DIR}/plugin-${wdp_UUID}.spec" CACHE STRING "Def.")

  # Define the library.
  add_library("${${_local}_TARGET}" SHARED ${wdp_SOURCES})
endfunction(wintermute_plugin_declare)

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
  set(_validProperties NAME_AUTHOR
                       NAME_RPC
                       EMAIL
                       URI
  )

  set(_options )
  set(_singleVals ${_validProperties} TARGET)
  cmake_parse_arguments(wpc "${_options}" "${_singleVals}" "" ${ARGN})

  string(TOUPPER "WINTERMUTE_PLUGIN_${wpc_TARGET}_" _local)
  
  foreach(_validProperty ${_validProperties})
    set("${_local}${_validProperty}" "${wpc_${_validProperty}}")
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

  set(_systemVersions  SYSTEM_VERSION_MAJOR
                      SYSTEM_VERSION_MINOR
                      SYSTEM_VERSION_PATCH
                      SYSTEM_VERSION_STAGE
                      SYSTEM_VERSION_COMPARISON
  )

  set(_singleVals ${_pluginVersions} ${_systemVersions} TARGET)
  cmake_parse_arguments(wpsv "" "${_singleVals}" "" ${ARGN})
  string(TOUPPER "WINTERMUTE_PLUGIN_${wpsv_TARGET}_" _local)

  foreach(_pluginVersionVariable ${_pluginVersions})
   set("${_local}${_pluginVersionVariable}" "${wpsv_${_pluginVersionVariable}}")
  endforeach(_pluginVersionVariable ${_pluginVersions})

  foreach(_systemVersionVariable ${_systemVersions})
   set("${_local}${_systemVersionVariable}" "${wpsv_${_systemVersionVariable}}")
  endforeach(_systemVersionVariable ${_systemVersions})

endfunction(wintermute_plugin_set_version)

##
## @fn wintermute_plugin_install
## @brief Enacts the necessary work to handle the installation process of 
##        a plug-in for Wintermute.
##
function(wintermute_plugin_install)
  cmake_parse_arguments(wpi "" "TARGET" "" ${ARGN})

  # DONE: Install the library itself.
  string(TOUPPER "WINTERMUTE_PLUGIN_${wpi_TARGET}" _local)
  install(TARGETS        ${${_local}_TARGET}
    LIBRARY DESTINATION  ${WINTERMUTE_PLUGIN_LIBRARY_DIR}  
  )

  # TODO: Install build-time headers.
  # TODO: Install documentation.
  # TODO: Generate plug-in definition file.
  # TODO: Install plug-in definition file.
  configure_file(${WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE} ${WINTERMUTE_PLUGIN_${wpi_TARGET}_DEFINITION_FILE} @ONLY)
endfunction(wintermute_plugin_install)
