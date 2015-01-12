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
PROJECT(WintermuteCore CXX)

include(WintermuteMacros)

set(_srcs
  # == Natives
  ./logging.cpp
  ./version.cpp

  # == Utilities
  ./util/serializable.cpp

  # == Plugin Related
  ./library.cc
  ./plugin.cc
  ./library.cpp
  ./plugin.cpp

  # == RPC Related
  ./tunnel.cc
  ./tunnel_dispatcher.cc
  ./tunnel_receiver.cc
  ./tunnel.cpp
  ./tunnel_dispatcher.cpp
  ./tunnel_receiver.cpp

  # == Procedure Related
  ./message.cc
  ./call.cc
  ./method.cc
  ./message.cpp
  ./call.cpp
  ./method.cpp

  # == Module Related
  ./module.cc
  ./module_pool.cc
  ./module_designation.cc
  ./module.cpp
  ./module_designation.cpp
  ./module_pool.cpp
  ./module_call.cpp

  # == Event related
  ./event_loop.cpp
  ./event_emitter.cpp
  ./event_object.cpp
  ./event_listener.cpp
  )

# Set up sources.
CONFIGURE_FILE(./globals.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/globals.hpp @ONLY)

ADD_LIBRARY(wintermute SHARED ${_srcs})
WINTERMUTE_ADD_TARGET_PROPERTIES(wintermute)
WINTERMUTE_LINK_LIBRARIES(wintermute gcov)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_BINARY_DIR}
  )

SET_TARGET_PROPERTIES(wintermute PROPERTIES
  DEFINE_SYMBOL WINTERMUTE_EXPORT
  VERSION       ${WINTERMUTE_VERSION} 
  )

file(GLOB _extra_doc_files
  "${CMAKE_SOURCE_DIR}/doc/wintermutecore/*.markdown")

DOXYGEN_GENERATE_DOCUMENTATION(
  TARGET           wintermute
  BUILD_AFTER      wintermute
  PROJECT_NAME     Wintermute
  PROJECT_BRIEF    "Wintermute's core library"
  PROJECT_VERSION  ${WINTERMUTE_VERSION}
  MAIN_DOC_PAGE    ${CMAKE_SOURCE_DIR}/doc/wintermutecore/Main.markdown
  SOURCE_DIR       ${CMAKE_CURRENT_SOURCE_DIR}
  EXTRA_DOC_FILES  ${_extra_doc_files}
)

# Define symbols for deprecation.
GENERATE_EXPORT_HEADER(wintermute
  PREFIX_NAME WNTR
  EXPORT_MACRO_NAME WINTERMUTE_EXPORT
  EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/wintermute_export.hpp
  )
