##############################################################################
### Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
###
### This file is part of Wintermute, the extensible AI platform.
###
### Wintermute is free software; you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation; either version 3 of the License, or
### (at your option) any later version.
###
### Wintermute is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
##############################################################################
option(WITH_ZEROMQ "Builds the ZeroMQ plug-in." OFF)
option(WITH_DBUS   "Builds the D-Bus plug-in."  QT_QTDBUS_FOUND)

add_feature_info(QtDBus WITH_DBUS "Provides the D-Bus RPC extension.")
add_feature_info(QtZeroMQ WITH_ZEROMQ "Provides the ZeroMQ RPC extension.")

add_subdirectory ( heartbeat )
add_subdirectory ( daemon )

if (WITH_DBUS)
  add_subdirectory ( dbus )
endif()

if (WITH_ZEROMQ)
  add_subdirectory ( zeromq )
endif()

