# - Utilities for generating documentation with Doxygen.
#
# TODO: Document.
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
INCLUDE(GNUInstallDirs)

# == Query if documentation should be generated.
OPTION(GENERATE_DOCUMENTATION ON "Generate documentation using Doxygen.")

IF (NOT GENERATE_DOCUMENTATION)
  macro(doxygen_generate_documentation)
  endmacro()
  MESSAGE(STATUS "Not generating documentation.")
  RETURN()
ENDIF()

FIND_PACKAGE(Doxygen 1.7 REQUIRED)

IF (NOT DOXYGEN_FOUND)
  MESSAGE(FATAL_ERROR "Cannot generate documentation, Doxygen not found.")
ENDIF()

# == Look for dot for graphing.
IF (NOT DOXYGEN_DOT_FOUND OR NOT DOXYGEN_DOT_EXECUTABLE)
  FIND_PROGRAM(DOT_PROGRAM dot
    DOC "Graphviz application for drawing graphs.")

  IF (DOT_PROGRAM-NOTFOUND)
    MESSAGE(WARNING
      "Cannot find Graphviz; no graphs will be drawn in documentation.")
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

  GET_TARGET_PROPERTY(_doxy_compile_options_raw ${_doxy_TARGET}
    COMPILE_OPTIONS)
  string(REPLACE ";" " " _doxy_compile_options
    "${_doxy_compile_options_raw}")

  SET(_docfiles ${_doxy_EXTRA_DOC_FILES})
  SET(_doxy_friendly_files )

  FOREACH(_docfile ${_docfiles})
    SET(_docfile "\"${_docfile}\"")
    SET(_doxy_friendly_files "${_doxy_friendly_files}${_docfile} \\\n")
  ENDFOREACH()

  SET(_doxy_EXTRA_DOC_FILES ${_doxy_friendly_files})

  SET(_doxy_working_dir
    "${CMAKE_BINARY_DIR}/docs/${_doxy_TARGET}")
  SET(_doxy_working_layout_dir
    "${CMAKE_BINARY_DIR}/docs-layout/${_doxy_TARGET}")

  FILE(MAKE_DIRECTORY ${_doxy_working_dir})
  FILE(MAKE_DIRECTORY ${_doxy_working_layout_dir})

  SET(_doxy_log_file
    "${CMAKE_BINARY_DIR}/${_doxy_TARGET}.doxygen.log")

  SET(_files_for_doxygen
    layout/header.html
    layout/footer.html
    layout/page.css
    layout/layout.xml
    )

  FOREACH(_file ${_files_for_doxygen})
    CONFIGURE_FILE("${WINTERMUTE_CMAKE_DIR}/Templates/doc/${_file}"
      "${_doxy_working_layout_dir}/doc/${_file}" @ONLY)
  ENDFOREACH()

  SET(_doxy_config_path
    "${_doxy_working_layout_dir}/cmake/${_doxy_TARGET}.dox")

  CONFIGURE_FILE(
    "${WINTERMUTE_CMAKE_DIR}/Templates/Doxyfile.in"
    "${_doxy_config_path}" @ONLY)

  SET(_doxy_args "${_doxy_config_path}")

  ADD_CUSTOM_TARGET("${_doxy_TARGET}-docs"
    COMMAND ${DOXYGEN_EXECUTABLE} ${_doxy_args}
    DEPENDS ${_doxy_BUILD_AFTER}
    WORKING_DIRECTORY ${_doxy_working_dir}
  )

  ADD_DEPENDENCIES(docs "${_doxy_TARGET}-docs")

  INSTALL(DIRECTORY ${_doxy_working_dir}/html/
    DESTINATION ${WINTERMUTE_DOCS_DIR}/${_doxy_TARGET}
    COMPONENT development
    CONFIGURATIONS Debug
  )

  INSTALL(DIRECTORY ${_doxy_working_dir}/man/man3/
    DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/man/man3
    COMPONENT development
    CONFIGURATIONS Debug
    FILES_MATCHING
      PATTERN "Wintermute*.3"
  )
ENDMACRO(doxygen_generate_documentation)
