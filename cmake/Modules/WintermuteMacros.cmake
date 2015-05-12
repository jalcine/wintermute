# vim: set ts=2 sts=2 sw=2 fdm=indent
###############################################################################
# Author: Jacky Alciné <me@jalcine.me>
#
# Wintermute is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.

# Wintermute is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with Wintermute; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
###############################################################################
include(WintermuteDependencies)
include(CMakeParseArguments)
include(UseWintermute)

#==============================================================================
#== @macro wintermute_add_target_properties
#== @param _target The target in question.
#== Takes a TARGET and applies Wintermute-specific properties to it. This
#== includes things that can vary on an environmental level as well.
#==============================================================================
MACRO(wintermute_add_target_properties _target)
  TARGET_COMPILE_OPTIONS(${_target} PUBLIC
    ${WINTERMUTE_COMPILE_FLAGS})
  TARGET_COMPILE_DEFINITIONS(${_target} PUBLIC
    ${WINTERMUTE_COMPILE_DEFINITIONS})
  TARGET_INCLUDE_DIRECTORIES(${_target} BEFORE PUBLIC
    ${WINTERMUTE_INCLUDE_DIRS})

  set(_ld_flags_raw "${WINTERMUTE_LINK_FLAGS}")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(_ld_flags_raw "${WINTERMUTE_LINK_FLAGS};${WINTERMUTE_LINK_FLAGS_DEBUG}")
  endif()
  string(REPLACE ";" " " _ld_flags "${_ld_flags_raw}")
  SET_TARGET_PROPERTIES(${_target} PROPERTIES
    LINK_FLAGS "${_ld_flags}"
    )

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    TARGET_COMPILE_OPTIONS(${_target} PUBLIC
      ${WINTERMUTE_COMPILE_FLAGS_DEBUG})
    TARGET_COMPILE_DEFINITIONS(${_target} PUBLIC
      ${WINTERMUTE_COMPILE_DEFINITIONS_DEBUG})
  endif()
ENDMACRO(wintermute_add_target_properties)

#==============================================================================
#== @macro wintermute_link_libraries
#== @param _target The target in question.
#== Links TARGET with the libraries Wintermute would need in order to operate.
#==============================================================================
MACRO(wintermute_link_libraries _target)
  IF (CMAKE_BUILD_TYPE STREQUAL "Debug" AND WINTERMUTE_TEST_LIBRARIES)
    TARGET_LINK_LIBRARIES(${_target}
      debug ${WINTERMUTE_TEST_LIBRARIES})
  ENDIF()

  TARGET_LINK_LIBRARIES(${_target}
    general ${WINTERMUTE_LIBRARIES})
ENDMACRO(wintermute_link_libraries _target)

MACRO(wintermute_install_target)
  SET(options
    )
  SET(oneValueArgs
    TARGET
    HEADER_DIR
    )
  SET(multiValueArgs
    HEADERS
    )

  CMAKE_PARSE_ARGUMENTS(_wit "${options}"
    "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  install(TARGETS "${_wit_TARGET}"
    LIBRARY DESTINATION ${WINTERMUTE_LIBRARY_DIR}
    ARCHIVE DESTINATION ${WINTERMUTE_LIBRARY_DIR}
    RUNTIME DESTINATION ${WINTERMUTE_BINARY_DIR}
    COMPONENT runtime
  )

  # TODO: Preserve directory structure.
  install(FILES ${_wit_HEADERS}
    DESTINATION ${WINTERMUTE_INCLUDE_DIR}/${_wit_HEADER_DIR}
    COMPONENT development
  )
ENDMACRO(wintermute_install_target)
