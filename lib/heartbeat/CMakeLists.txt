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
###############################################################################
project ( WintermuteHeartbeat )
set ( heartbeat_SRCS
  plugin.cpp
  module.cpp
  calls/greet.cpp
)

set ( heartbeat_HDRS
  plugin.hpp
  module.hpp
  calls/greet.hpp
)

configure_file ( globals.hpp.in ${CMAKE_SOURCE_DIR}/lib/heartbeat/globals.hpp
  @ONLY )

wintermute_plugin_declare ( TARGET heartbeat SOURCES ${heartbeat_SRCS} )
wintermute_plugin_set_version ( TARGET heartbeat
  PLUGIN_VERSION_MAJOR 0
  PLUGIN_VERSION_MINOR 1
  PLUGIN_VERSION_PATCH 0
  PLUGIN_VERSION_STAGE dev

  SYSTEM_VERSION_MAJOR ${WINTERMUTE_VERSION_MAJOR}
  SYSTEM_VERSION_MINOR ${WINTERMUTE_VERSION_MINOR}
  SYSTEM_VERSION_PATCH ${WINTERMUTE_VERSION_PATCH}
  SYSTEM_VERSION_STAGE ${WINTERMUTE_VERSION_STAGE} )

wintermute_plugin_configure ( TARGET heartbeat
  AUTHOR_NAME "Jacky Alciné"
  NAME_RPC "wintermute.in.heartbeat"
  EMAIL "me@jalcine.me"
  URI "http://wintermute.jalcine.me/plugins/heartbeart.html"
  )
wintermute_plugin_target_declare ( TARGET heartbeat SOURCES ${heartbeat_SRCS} )
include_directories ( ${Wintermute_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
wintermute_plugin_install ( TARGET heartbeat )
