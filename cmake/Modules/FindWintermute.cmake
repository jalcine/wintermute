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

include(${CMAKE_CURRENT_LIST_DIR}/UseWintermute.cmake OPTIONAL)

if(NOT WINTERMUTE_FOUND)
  message(FATAL_ERROR "Wintermute was not found at '${CMAKE_CURRENT_LIST_DIR}'.")
else()
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/Internal)
  include(WintermuteVariables)
  include(WintermuteDependencies)
  include(WintermuteVersion)
  include(WintermuteMacros)
  message(STATUS "Wintermute development enabled.")
endif()
