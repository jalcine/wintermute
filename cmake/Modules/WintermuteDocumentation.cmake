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

option(WITH_DOCUMENTATION ON "Generate documentation for the project.")

if (WITH_DOCUMENTATION)
  find_package(Doxygen 1.8.6 REQUIRED)

  include(CMakeParseArguments)
  include(WintermuteVariables)

  set(WINTERMUTE_DOXYFILE_TEMPLATE "${WINTERMUTE_TEMPLATES_DIR}/Doxyfile.in")
endif()

macro(wintermute_generate_documentation)
  set(_singleArgs TARGET NAME VERSION BRIEF OUTPUT_DIRECTORY)
  set(_multiArgs SOURCES)

  cmake_parse_arguments(wgd "" "${_singleArgs}" "${_multiArgs}" ${ARGN})

  if (NOT DOXYGEN_FOUND OR NOT WITH_DOCUMENTATION)
    message(STATUS "Not making documentation for ${wgd_TARGET}.")
    return()
  endif()

  set(_local_target "${wgd_TARGET}-doc")
  if (NOT TARGET ${_local_target})
    string(TOUPPER "WINTERMUTE_PLUGIN_${wgd_TARGET}" _local)

    set(${_local}_DOXYFILE "${CMAKE_BINARY_DIR}/Doxyfile.${wgd_TARGET}")
    configure_file(${WINTERMUTE_DOXYFILE_TEMPLATE} ${${_local}_DOXYFILE} @ONLY)

    add_custom_command(TARGET ${wgd_TARGET}
      COMMAND           ${DOXYGEN_EXECUTABLE} ${${_local}_DOXYFILE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT           "Generating documentation for ${wgd_TARGET}..."
    )
  endif()
endmacro(wintermute_generate_documentation)

## Define the features provided for documentation.
if (NOT DEFINED _hax_dox)
  set(_hax_dox OFF)
  if (WITH_DOCUMENTATION AND DOXYGEN_FOUND)
    set(_hax_dox ON)
  endif()

  set_package_properties(Doxygen PROPERTIES TYPE RECOMMENDED)

  add_feature_info(Documentation _hax_dox 
    "Documentation generation tool to HTML, QHP, and man pages.")
endif()
