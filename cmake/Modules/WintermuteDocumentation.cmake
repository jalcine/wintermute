# TODO: Add proper CMake module header definitions.
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

find_package(Doxygen REQUIRED)

include(CMakeParseArguments)
include(WintermuteVariables)

if (NOT TARGET wintermute)
  set(WINTERMUTE_DOXYFILE_TEMPLATE 
    "${WINTERMUTE_CMAKE_TEMPLATES_DIR}/Doxyfile.in")
else()
  set(WINTERMUTE_DOXYFILE_TEMPLATE 
    "${WINTERMUTE_CMAKE_TEMPLATES_INSTALL_DIR}/Doxyfile.in")
endif()

macro(wintermute_generate_documentation)
  set(_singleArgs TARGET NAME VERSION BRIEF OUTPUT_DIRECTORY)
  set(_multiArgs SOURCES)

  cmake_parse_arguments(wgd "" "${_singleArgs}" "${_multiArgs}" ${ARGN})

  set(_local_target ${wgd_TARGET}-doc)
  string(TOUPPER "WINTERMUTE_PLUGIN_${wgd_TARGET}" _local)

  # DONE: Configure the Doxygen configuration file.
  set(${_local}_DOXYFILE "${CMAKE_BINARY_DIR}/Doxyfile.${${_local}_TARGET}")
  configure_file(${WINTERMUTE_DOXYFILE_TEMPLATE} ${${_local}_DOXYFILE})

  # DONE: Define a target for Doxygen to execute.
  # DONE: Make the documentation target dependent on a parent target.
  add_custom_command(TARGET ${_local_target} PRE_BUILD
    DEPENDS           ${wgd_TARGET}
    COMMAND           ${DOXYGEN_EXECUTABLE}
    ARGS              ${${_local}_DOXYFILE}
    DEPENDS           ${${_local}_TARGET}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endmacro(wintermute_generate_documentation)
