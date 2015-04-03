# vim: set ts=2 sts=2 sw=2 fdm=marker
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

# = Look up package-level dependencies.
INCLUDE(FindPkgConfig)
INCLUDE(WintermuteVariables)

PKG_CHECK_MODULES(JsonCpp jsoncpp REQUIRED)
PKG_CHECK_MODULES(Log4Cxx liblog4cxx REQUIRED)
PKG_CHECK_MODULES(LibUv libuv REQUIRED)
PKG_CHECK_MODULES(Uuid uuid REQUIRED)
PKG_CHECK_MODULES(LibConfig libconfig++ REQUIRED)

# == Exported variables
set(WINTERMUTE_INCLUDE_DIRS
  ${JsonCpp_INCLUDE_DIRS}
  ${Log4Cxx_INCLUDE_DIRS}
  ${LibUv_INCLUDE_DIRS}
  ${LibConfig_INCLUDE_DIRS}
  ${Uuid_INCLUDE_DIRS}
  )

set(WINTERMUTE_LIBRARIES
  ${JsonCpp_LIBRARIES}
  ${Log4Cxx_LIBRARIES}
  ${LibUv_LIBRARIES}
  ${LibConfig_LIBRARIES}
  ${Uuid_LIBRARIES}
  )

list(APPEND WINTERMUTE_COMPILE_FLAGS
  ${JsonCpp_CFLAGS}
  ${Log4Cxx_CFLAGS}
  ${LibUv_CFLAGS}
  ${LibConfig_CLAGS}
  ${Uuid_CFLAGS}
  )

list(APPEND WINTERMUTE_LINK_FLAGS
  ${JsonCpp_LDFLAGS}
  ${Log4Cxx_LDFLAGS}
  ${LibUv_LDFLAGS}
  ${LibConfig_LDFLAGS}
  ${Uuid_LDFLAGS}
  )

# == Add to the required varibles to improve searching.
set(CMAKE_REQUIRED_INCLUDES ${WINTERMUTE_INCLUDE_DIRS})
set(CMAKE_REQUIRED_FLAGS ${WINTERMUTE_LINK_FLAGS} ${WINTERMUTE_COMPILE_FLAGS})

# == Extra inclusion.
include(WintermuteFunctionDependencies)
include(WintermuteFlagDependencies)
