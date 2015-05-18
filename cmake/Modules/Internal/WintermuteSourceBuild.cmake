# vim: set ts=2 sts=2 sw=2 fdm=marker
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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

#== Preserve some older paths.
if (NOT WINTERMUTE_INSTALL_CMAKE_MODULE_DIR AND DEFINED WINTERMUTE_CMAKE_DIR)
  set(WINTERMUTE_INSTALL_CMAKE_MODULE_DIR
    ${WINTERMUTE_CMAKE_DIR}/Modules CACHE INTERNAL "" FORCE)
  set(WINTERMUTE_INSTALL_CMAKE_TEMPLATE_DIR
    ${WINTERMUTE_CMAKE_DIR}/Templates CACHE INTERNAL "" FORCE)
endif()

#== Override other paths.
SET(WINTERMUTE_CMAKE_DIR ${CMAKE_SOURCE_DIR}/cmake
  CACHE INTERNAL "Localized CMake modules directory." FORCE)
SET(WINTERMUTE_SOURCE_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/src
  PARENT_SCOPE)
SET(WINTERMUTE_TEST_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/test/include
  ${CMAKE_SOURCE_DIR}/test
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/src
  PARENT_SCOPE)

set(WINTERMUTE_CMAKE_MODULE_DIR
  ${WINTERMUTE_CMAKE_DIR}/Modules CACHE INTERNAL "" FORCE)
set(WINTERMUTE_CMAKE_TEMPLATE_DIR
  ${WINTERMUTE_CMAKE_DIR}/Templates CACHE INTERNAL "" FORCE)
