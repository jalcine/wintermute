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

INCLUDE(CMakeParseArguments)
OPTION(GENERATE_DOCUMENTATION OFF "Generate documentation using Doxygen.")

IF (GENERATE_DOCUMENTATION EQUAL OFF)
  return()
ENDIF()

FIND_PACKAGE(Doxygen 1.8 REQUIRED)

IF (NOT DOXYGEN_FOUND)
  MESSAGE("Cannot generate documentation, Doxygen not found.")
ENDIF()

# Look for dot for graphing.
IF (NOT DOXYGEN_DOT_FOUND OR NOT DOXYGEN_DOT_EXECUTABLE)
  FIND_PROGRAM(DOT_PROGRAM dot
    DOC "Graphviz application for drawing graphs.")

  IF (DOT_PROGRAM-NOTFOUND)
    MESSAGE("Cannot find Graphviz; no graphs will be drawn in documentation.")
    set(DOT_PROGRAM "")
  ELSE()
    SET(DOXYGEN_DOT_EXECUTABLE ${DOT_PROGRAM})
  ENDIF()
ENDIF()

ADD_CUSTOM_TARGET(docs)

MACRO(doxygen_generate_documentation)
  SET(options
    )
  SET(oneValueArgs
    TARGET
    BUILD_AFTER
    PROJECT_NAME
    PROJECT_VERSION
    PROJECT_BRIEF
    MAIN_DOC_PAGE
    PROJECT_LOGO
    EXAMPLE_PATH
    IMAGE_PATH
    SOURCE_DIR
    )
  SET(multiValueArgs
    SOURCES
    EXTRA_DOC_FILES
    )

  CMAKE_PARSE_ARGUMENTS(_doxy "${options}"
    "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(_docfiles ${_doxy_EXTRA_DOC_FILES})
  set(_doxy_friendly_files )

  foreach(_docfile ${_docfiles})
    set(_docfile "\"${_docfile}\"")
    set(_doxy_friendly_files "${_doxy_friendly_files}${_docfile} \\\n")
  endforeach()

  set(_doxy_EXTRA_DOC_FILES ${_doxy_friendly_files})

  SET(_doxy_working_dir
    "${CMAKE_BINARY_DIR}/docs/${_doxy_TARGET}")
  SET(_doxy_working_layout_dir
    "${CMAKE_BINARY_DIR}/docs-layout/${_doxy_TARGET}")

  file(MAKE_DIRECTORY ${_doxy_working_dir})
  file(MAKE_DIRECTORY ${_doxy_working_layout_dir})

  SET(_doxy_log_file
    "${CMAKE_BINARY_DIR}/${_doxy_TARGET}.doxygen.log")

  set(_files_for_doxygen
    layout/header.html
    layout/footer.html
    layout/page.css
    layout/layout.xml
    )

  foreach(_file ${_files_for_doxygen})
    CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/doc/${_file}"
      "${_doxy_working_layout_dir}/doc/${_file}" @ONLY)
  endforeach()

  SET(_doxy_config_path
    "${_doxy_working_layout_dir}/cmake/${_doxy_TARGET}.dox")

  CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/Doxyfile.in"
    "${_doxy_config_path}" @ONLY)

  SET(_doxy_args "${_doxy_config_path}")

  ADD_CUSTOM_TARGET("${_doxy_TARGET}-docs" ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${_doxy_args}
    DEPENDS ${_doxy_BUILD_AFTER}
    WORKING_DIRECTORY ${_doxy_working_dir}
  )

  add_dependencies(docs "${_doxy_TARGET}-docs")
ENDMACRO(doxygen_generate_documentation)
