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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(WintermtueTransportZeroMQ CXX)

INCLUDE(WintermutePlugin)
FIND_PACKAGE(ZeroMQCXX REQUIRED)

SET(WINTERMUTE_ZMQ_VERSION     0.0.1-dev)
SET(WINTERMUTE_ZMQ_IPC_ADDRESS /tmp/wintermute.sock)

SET(_srcs
  plugin.cpp
  dispatcher.cpp
  receiver.cpp
  )

CONFIGURE_FILE(globals.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/globals.hpp
  @ONLY
  )

ADD_LIBRARY(wintermute-transport-zmq
  SHARED ${_srcs}
  )

TARGET_LINK_LIBRARIES(wintermute-transport-zmq
  ${ZEROMQ_LIBS} zmqpp zmq
  )

TARGET_INCLUDE_DIRECTORIES(wintermute-transport-zmq
  PUBLIC ${ZEROMQ_INCLUDE_DIRS}
)

WINTERMUTE_PLUGIN_DECLARE(
  NAME   wintermute-transport-zmq
  TARGET wintermute-transport-zmq
  VERSION ${WINTERMUTE_ZMQ_VERSION}
  )

WINTERMUTE_PLUGIN_VALIDATE(TARGET wintermute-transport-zmq)

ADD_SUBDIRECTORY(test)
