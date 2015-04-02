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
if (NOT DEFINED _wntr_ver)
  set(_wntr_ver ON)
else()
  return()
endif()

set(WINTERMUTE_VERSION_MAJOR 0)
set(WINTERMUTE_VERSION_MINOR 0)
set(WINTERMUTE_VERSION_PATCH 1)
set(WINTERMUTE_VERSION_RC "dev")
set(WINTERMUTE_VERSION
  ${WINTERMUTE_VERSION_MAJOR}.${WINTERMUTE_VERSION_MINOR}.${WINTERMUTE_VERSION_PATCH}-${WINTERMUTE_VERSION_RC})

message(STATUS "Wintermute ${WINTERMUTE_VERSION} is being built.")
