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

# == Set variables to make Wintermute building a bit more sane.
set(BUILD_SHARED_LIBRARIES ON PARENT_SCOPE)
set(CMAKE_COLOR_MAKEFILE ON PARENT_SCOPE)
set(CMAKE_VERBOSE_MAKEFILE OFF PARENT_SCOPE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON PARENT_SCOPE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin PARENT_SCOPE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib PARENT_SCOPE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib PARENT_SCOPE)

# == Allow this project to use the modules we've provided.
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}/
  ${CMAKE_SOURCE_DIR}/cmake/Modules/
  ${CMAKE_SOURCE_DIR}/cmake/Modules/Internal/
  CACHE INTERNAL "Include paths."
  )

INCLUDE(WintermuteSourceBuild)
