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

function(wintermute_plugin_declare)
  set(_oneArgs   TARGET)
  set(_multiArgs SOURCES)
  cmake_parse_arguments(wdp "" "${_oneArgs}" "${_multiArgs}" ${ARGN})

  # Define the plugin's CMake prefix.
  string(TOUPPER "${wdp_TARGET}" wdp_TARGET)
  set(_local "WINTERMUTE_PLUGIN_${wdp_TARGET}")

  # Define the plugin's CMake properties.
  set("${_local}_SOURCES" "${wdp_SOURCES}"           CACHE STRING "Sources.")
  set("${_local}_TARGET"  "wintermute-${wdp_TARGET}" CACHE STRING "Target.")
  string(TOLOWER "${${_local}_TARGET}" "${_local}_TARGET")

  # Define the library.
  add_library("${${_local}_TARGET}" SHARED ${wdp_SOURCES})
endfunction(wintermute_plugin_declare)

function(wintermute_plugin_set_properties)
endfunction(wintermute_plugin_set_properties)

function(wintermute_plugin_get_property)
endfunction(wintermute_plugin_get_property)

function(wintermute_plugin_install)
  set(_oneArgs  TARGET)
  cmake_parse_arguments(wpi "" "${_oneArgs}" "" ${ARGN})

  install(TARGETS      ${wpi_TARGET}
    LIBRARY DESTINATION  ${WINTERMUTE_PLUGIN_LIBRARY_DIR}  
  )
endfunction(wintermute_plugin_install)
