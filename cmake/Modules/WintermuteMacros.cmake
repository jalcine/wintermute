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
include(WintermuteDocumentation)

macro(wintermute_add_properties _target)
  set_target_properties(${_target} PROPERTIES
    COMPILE_DEFINITIONS         "${WINTERMUTE_COMPILE_DEFINITIONS}"
    COMPILE_DEFINITIONS_DEBUG   "${WINTERMUTE_COMPILE_DEFINITIONS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${WINTERMUTE_COMPILE_DEFINITIONS_RELEASE}"
    LINK_FLAGS                  "${WINTERMUTE_LINK_FLAGS}"
    LINK_FLAGS_DEBUG            "${WINTERMUTE_LINK_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${WINTERMUTE_LINK_FLAGS_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY    "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY    "${CMAKE_BINARY_DIR}/bin"
  )

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(TARGET ${_target} PROPERTY COMPILE_FLAGS 
      "${WINTERMUTE_COMPILE_FLAGS} ${WINTERMUTE_COMPILE_FLAGS_DEBUG}")
    set(CMAKE_VERBOSE_MAKEFILE ON)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET ${_target} PROPERTY COMPILE_FLAGS
      "${WINTERMUTE_COMPILE_FLAGS} ${WINTERMUTE_COMPILE_FLAGS_RELEASE}")
  else()
    set_property(TARGET ${_target} PROPERTY COMPILE_FLAGS "${WINTERMUTE_COMPILE_FLAGS}")
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

  ## Generate documentation.
  #get_target_property(_sources ${_target} SOURCES)
  #wintermute_generate_documentation(SOURCES ${_sources})
endmacro(wintermute_add_properties _target)

## Include plug-in specific macros.
include(WintermutePluginMacros)
