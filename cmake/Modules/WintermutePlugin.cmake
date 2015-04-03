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
INCLUDE(WintermuteTest)
INCLUDE(WintermuteVariables)
INCLUDE(WintermuteMacros)

MACRO(wintermute_plugin_declare)
  SET(options
    )
  SET(oneValueArgs
    NAME
    VERSION
    TARGET
    )
  SET(multiValueArgs
    INCLUDE_DIRECTORIES
    )

  CMAKE_PARSE_ARGUMENTS(_wpd "${options}"
    "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  WINTERMUTE_ADD_TARGET_PROPERTIES(${_wpd_TARGET})
  TARGET_LINK_LIBRARIES(${_wpd_TARGET} wintermute-core)
  if (DEFINED _wpd_VERSION)
    SET_TARGET_PROPERTIES(${_wpd_TARGET}
      PROPERTIES
        VERSION ${_wpd_VERSION}
        SO_VERSION ${_wpd_VERSION}
    )
  endif()

  SET(_include_dirs
    ${_wpd_INCLUDE_DIRECTORIES}
    ${WINTERMUTE_INCLUDE_DIRS}
    ${WINTERMUTE_SOURCE_INCLUDE_DIRS})

  # TODO: Handle extra inclusion directories via WintermuteSourceBuild.

  TARGET_INCLUDE_DIRECTORIES(${_wpd_TARGET} BEFORE
    PUBLIC ${_include_dirs})

  WINTERMUTE_INSTALL_TARGET(
    TARGET ${_wpd_TARGET}
  )
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
  SET(_test_driver_file
    "${WINTERMUTE_CMAKE_DIR}/Templates/plugin_driver.hh.in")

  GET_TARGET_PROPERTY(_wpv_file ${_wpv_TARGET} LOCATION)

  CONFIGURE_FILE(${_test_driver_file}
    ${_plugin_driver_file}
    @ONLY)

  WINTERMUTE_ADD_TEST(plugin-verify ${_wpv_TARGET} ${_plugin_driver_file})
ENDMACRO(wintermute_plugin_validate)

MACRO(wintermute_plugin_add_test)
  SET(options
    )
  SET(oneValueArgs
    PREFIX
    HEADER
    NAME
    TARGET
    )
  SET(multiValueArgs
    )

  CMAKE_PARSE_ARGUMENTS(_wpat "${options}"
    "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  WINTERMUTE_ADD_TEST("plugin-${_wpat_PREFIX}" ${_wpat_NAME} ${_wpat_HEADER})
  TARGET_LINK_LIBRARIES("plugin-${_wpat_PREFIX}-${_wpat_NAME}" ${_wpat_TARGET})
ENDMACRO(wintermute_plugin_add_test)
