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
project ( WintermuteZeroMQ )
find_package(QtZeroMQ)
set ( zeromq_SRCS
  plugin.cpp
  dispatcher.cpp
  module.cpp )

set ( zeromq_HDRS
  plugin.hpp
  dispatcher.hpp
  module.hpp )

configure_file ( globals.hpp.in ${CMAKE_SOURCE_DIR}/lib/zeromq/globals.hpp
  @ONLY )

wintermute_plugin_declare ( TARGET zeromq SOURCES ${zeromq_SRCS} )
wintermute_plugin_set_version ( TARGET zeromq
  PLUGIN_VERSION_MAJOR 0
  PLUGIN_VERSION_MINOR 1
  PLUGIN_VERSION_PATCH 0
  PLUGIN_VERSION_STAGE dev

  SYSTEM_VERSION_MAJOR ${WINTERMUTE_VERSION_MAJOR}
  SYSTEM_VERSION_MINOR ${WINTERMUTE_VERSION_MINOR}
  SYSTEM_VERSION_PATCH ${WINTERMUTE_VERSION_PATCH}
  SYSTEM_VERSION_STAGE ${WINTERMUTE_VERSION_STAGE} )
wintermute_plugin_configure ( TARGET zeromq
  AUTHOR_NAME "Jacky Alcine"
  NAME_RPC "me.jalcine.wintermute.zeromq"
  EMAIL "me@jalcine.me"
  URI "http://wintermute.jalcine.me/plugins/zeromq.html"
  )
wintermute_plugin_add_libraries ( TARGET zeromq LIBRARIES ${QTZERO_LIBRARIES})
wintermute_plugin_add_include_directories ( TARGET zeromq DIRECTORIES ${QTZEROMQ_INCLUDE_DIRS} )
wintermute_plugin_target_declare ( TARGET zeromq SOURCES ${zeromq_SRCS} )
wintermute_plugin_install ( TARGET zeromq )
