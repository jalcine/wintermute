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
install(WintermuteVariables)

function(wintermute_plugin_declare)
  set(_mutliArgs SOURCES)
  set(_oneArgs   NAME TARGET)

  cmake_parse_arguments(wdp "" "${_oneArgs}" "${_multiArgs}" ${ARGN})

  string(TOUPPER "${wdp_TARGET}" ${wdp_TARGET})
  set(_local "WINTERMUTE_PLUGIN_${wdp_TARGET}")

  set("${wdp_NAME}_TARGET" ${wdp_TARGET})

  add_library(${wdp_TARGET} SHARED ${wdp_SOURCES})
endfunction(wintermute_plugin_declare)

function(wintermute_plugin_set_properties)
endfunction(wintermute_plugin_set_properties)

function(wintermute_plugin_get_property)
function(wintermute_plugin_get_property)

function(wintermute_plugin_install)
  set(_oneArgs  TARGET)
  cmake_parse_arguments(wpi "" "${_oneArgs}" "" ${ARGN})

  install(TARGETS      ${wpi_TARGET}
    LIBRARY DESTINATION  ${WINTERMUTE_PLUGIN_LIBRARY_DIR}  
  )
function(wintermute_plugin_install)
