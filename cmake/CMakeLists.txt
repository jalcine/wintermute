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

# == Import prerequistie modules.
INCLUDE(GNUInstallDirs)

# == Set variables to make Wintermute building a bit more sane.
SET(BUILD_SHARED_LIBRARIES         ON  PARENT_SCOPE)
SET(CMAKE_COLOR_MAKEFILE           ON  PARENT_SCOPE)
SET(CMAKE_EXPORT_COMPILE_COMMANDS  ON  PARENT_SCOPE)
SET(CMAKE_VERBOSE_MAKEFILE         OFF PARENT_SCOPE)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin PARENT_SCOPE)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib PARENT_SCOPE)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib PARENT_SCOPE)

# == Allow this project to use the modules we've provided.
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_SOURCE_DIR}/cmake/Modules/
  ${CMAKE_SOURCE_DIR}/cmake/Modules/Internal/
  ${CMAKE_SOURCE_DIR}/cmake/Templates/
  ${CMAKE_SOURCE_DIR}/cmake/Templates/Internal/
  CACHE INTERNAL "Include paths."
  )

# == Generate 'UseWintermute.cmake'
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/Templates/Internal/UseWintermute.cmake.in
  ${CMAKE_CURRENT_SOURCE_DIR}/Modules/UseWintermute.cmake
  @ONLY)

INCLUDE(CMakeProvideUninstallTarget)

# == Install modules and templates.
file(GLOB _modules Modules/*.cmake)
file(GLOB _templates Templates/*.in)

INSTALL(FILES ${_modules}
  DESTINATION ${WINTERMUTE_INSTALL_CMAKE_MODULE_DIR}
)
INSTALL(FILES ${_templates}
  DESTINATION ${WINTERMUTE_INSTALL_CMAKE_TEMPLATE_DIR}
)
