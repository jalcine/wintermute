##############################################################################
### Copyright (C) 2013, 2014 Jacky Alciné <me@jalcine.me>
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
###############################################################################
project ( WintermuteDBus )
set ( dbus_SRCS 
  plugin.cpp
  module.cpp
  adaptor.cpp
  dispatcher.cpp
  receiver.cpp
)

set ( dbus_HDRS
  plugin.hpp
  dispatcher.hpp
  adaptor.hpp
  receiver.hpp
  module.hpp
  globals.hpp
)

configure_file ( globals.hpp.in ${CMAKE_SOURCE_DIR}/lib/dbus/globals.hpp
  @ONLY )

find_package ( Qt4 COMPONENTS 
  QtDBus
  REQUIRED)

# Only available with Qt5.
# pkg_search_module(QTDBUSTEST libqtdbustest-1 REQUIRED)

wintermute_plugin_declare (
  TARGET dbus
  HEADERS ${dbus_HDRS}
  SOURCES ${dbus_SRCS}
)
wintermute_plugin_set_version ( TARGET dbus
  PLUGIN_VERSION_MAJOR 0
  PLUGIN_VERSION_MINOR 1
  PLUGIN_VERSION_PATCH 0
  PLUGIN_VERSION_STAGE dev

  SYSTEM_VERSION_MAJOR ${WINTERMUTE_VERSION_MAJOR}
  SYSTEM_VERSION_MINOR ${WINTERMUTE_VERSION_MINOR}
  SYSTEM_VERSION_PATCH ${WINTERMUTE_VERSION_PATCH}
  SYSTEM_VERSION_STAGE ${WINTERMUTE_VERSION_STAGE} )
wintermute_plugin_configure ( TARGET dbus
  AUTHOR_NAME "Jacky Alcine"
  NAME_RPC "wintermute.in.dbus"
  EMAIL "me@jalcine.me"
  URI "http://wintermute.jalcine.me/plugins/dbus.html"
  )
wintermute_plugin_add_libraries ( TARGET dbus LIBRARIES ${QT_QTDBUS_LIBRARY})
wintermute_plugin_add_include_directories ( TARGET dbus DIRECTORIES
  ${QT_QTDBUS_INCLUDE_DIR} )
wintermute_plugin_target_declare ( TARGET dbus )
wintermute_plugin_install ( TARGET dbus )
