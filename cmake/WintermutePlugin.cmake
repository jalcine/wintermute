# vim: set ts=2 sts=2 sw=2 fdm=indent
###############################################################################
# Author: Jacky Alciné <me@jalcine.me>
#
# Wintermute is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
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

include(WintermuteSourceBuild
  OPTIONAL
  RESULT_VARIABLE WINTERMUTE_IS_SOURCE_BUILD
  )

include(WintermuteTest)

MACRO(wintermute_plugin_declare)
  SET(options
    )
  SET(oneValueArgs
    NAME
    TARGET
    )
  SET(multiValueArgs
    )

  CMAKE_PARSE_ARGUMENTS(_wpd "${options}"
    "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  target_link_libraries(${_wpd_TARGET}
    wintermutecore)

  set(_include_dirs )
  set(_libs )

  if (EXISTS ${WINTERMUTE_IS_SOURCE_BUILD})
    set(_include_dirs
      ${CMAKE_SOURCE_DIR}/src
      ${CMAKE_BINARY_DIR}/src/wintermutecore
      )
  else()
    set(_include_dirs ${WINTERMUTE_INCLUDE_DIR})
  endif()

  target_include_directories(${_wpd_TARGET} BEFORE
    PUBLIC ${_include_dirs})
ENDMACRO(wintermute_plugin_declare)

# TODO: Configure the test driver source file to work for the provided Plugin.
MACRO(wintermute_plugin_validate)
  SET(options
    )
  SET(oneValueArgs
    TARGET
    )
  SET(multiValueArgs
    )

  CMAKE_PARSE_ARGUMENTS(_wpv "${options}"
    "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  SET(_plugin_driver_file
    "${CMAKE_CURRENT_BINARY_DIR}/${_wpv_TARGET}-validator.hh")
  SET(_test_driver_file )

  IF (EXISTS ${WINTERMUTE_IS_SOURCE_BUILD})
    SET(_test_driver_file "${CMAKE_SOURCE_DIR}/cmake/plugin_driver.hh.in")
  ELSE()
  ENDIF()

  GET_TARGET_PROPERTY(_wpv_file ${_wpv_TARGET} LOCATION)

  CONFIGURE_FILE(${_test_driver_file}
    ${_plugin_driver_file}
    @ONLY)

  WINTERMUTE_ADD_TEST(plugin-verify ${_wpv_TARGET} ${_plugin_driver_file})
ENDMACRO(wintermute_plugin_validate)
