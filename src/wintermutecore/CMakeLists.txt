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
PROJECT(WintermuteCore CXX)

include(WintermuteMacros)

set(_srcs
  # Natives
  ./logging.cpp
  ./util/serializable.cpp
  ./version.cpp

  # Procedural
  ./message.cc
  ./message.cpp
  ./module.cpp
  ./module.cc
  ./module_designation.cc
  ./module_designation.cpp
  ./module_pool.cc
  ./module_pool.cpp
  #./dispatcher.cpp
  #./receiver.cpp
  #./tunnel.cpp

  # == Plugin related.
  ./library.cc
  ./library.cpp
  ./plugin.cc
  ./plugin.cpp
  )

# Set up sources.
CONFIGURE_FILE(./globals.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/globals.hpp @ONLY)

ADD_LIBRARY(wintermutecore SHARED ${_srcs})
WINTERMUTE_ADD_TARGET_PROPERTIES(wintermutecore)
WINTERMUTE_LINK_LIBRARIES(wintermutecore gcov)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_BINARY_DIR}
  )

SET_TARGET_PROPERTIES(wintermutecore PROPERTIES
  DEFINE_SYMBOL "WINTERMUTE_EXPORT"
  )


file(GLOB _extra_doc_pages
  doc/wintermutecore/**/*.markdown)

DOXYGEN_GENERATE_DOCUMENTATION(
  TARGET           "wintermutecore"
  BUILD_AFTER      "wintermutecore"
  PROJECT_NAME     "Wintermute"
  PROJECT_BRIEF    "Wintermute's core library"
  PROJECT_VERSION  "${WINTERMUTE_VERSION}"
  MAIN_DOC_PAGE    "${CMAKE_SOURCE_DIR}/doc/wintermutecore/Main.markdown"
  SOURCE_DIR       "${CMAKE_CURRENT_SOURCE_DIR}"
  EXTRA_DOC_FILES  ${_extra_doc_pages}
)
message("${CMAKE_SOURCE_DIR}/doc/Main.markdown")

# Define symbols for deprecation.
GENERATE_EXPORT_HEADER(wintermutecore
  PREFIX_NAME WNTR
  EXPORT_MACRO_NAME WINTERMUTE_EXPORT
  EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/wintermute_export.hpp
  )
