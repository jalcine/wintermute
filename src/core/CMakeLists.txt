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
PROJECT(WintermuteCoreLibrary)

# {{{ Sources
set(CMAKE_AUTOMOC ON)
SET(wintermute-core_sources
  version.cpp
  plugin.cpp
  driver.cpp
  )

CONFIGURE_FILE(export.hpp.in export.hpp @ONLY)
# }}}

# {{{ Dependencies
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_SEARCH_MODULE(Qt REQUIRED QtCore)
# }}}

# {{{ Binary generation
INCLUDE_DIRECTORIES(${Qt_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
ADD_LIBRARY(wintermute-core SHARED ${wintermute-core_sources})
# }}}
