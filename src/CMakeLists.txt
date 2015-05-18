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
# == Enforce source building.
INCLUDE(WintermuteSourceBuild)

# == Root library.
ADD_SUBDIRECTORY(wintermute-core)

# == Standalone Plugins
ADD_SUBDIRECTORY(wintermute-daemon)
ADD_SUBDIRECTORY(wintermute-heartbeat)

# == Transports
#ADD_SUBDIRECTORY(wintermute-transport-dbus)
ADD_SUBDIRECTORY(wintermute-transport-zeromq)
#ADD_SUBDIRECTORY(wintermute-transport-http)
#ADD_SUBDIRECTORY(wintermute-transport-gcm)
