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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

if (NOT DEFINED _wntr_deps)
  set(_wntr_deps ON)
else()
  return()
endif()

# == Look up manual dependencies.
INCLUDE(WintermuteHeaderDependencies)

# == Look up package-level dependencies.
INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(JsonCpp jsoncpp REQUIRED)
PKG_SEARCH_MODULE(Log4Cxx liblog4cxx REQUIRED)

# Look for Boost and the aspects we'd like from it.
FIND_PACKAGE(Boost 1.54 EXACT REQUIRED
  COMPONENTS filesystem system)

# == Exported variables
set(WINTERMUTE_INCLUDE_DIRS
  ${Boost_FILE_SYSTEM_INCLUDE_DIRS}
  ${Boost_SYSTEM_INCLUDE_DIRS}
  ${JsonCpp_INCLUDE_DIRS}
  ${Log4Cxx_INCLUDE_DIRS}
  )

set(WINTERMUTE_LIBRARIES
  dl
  ${Boost_FILESYSTEM_LIBRARY_RELEASE}
  ${Boost_SYSTEM_LIBRARY_RELEASE}
  ${JsonCpp_LIBRARIES}
  ${Log4Cxx_LIBRARIES}
  )

if (DEFINED WINTERMUTE_COMPILE_FLAGS)
  set(WINTERMUTE_COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS}
    ${JsonCpp_CFLAGS}
    ${Log4Cxx_CFLAGS}
    )
endif()
